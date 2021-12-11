//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

// System includes
#include <algorithm>

// Project includes
#include "includes/communication/local_socket_communication.hpp"

namespace CoSimIO {
namespace Internals {


LocalSocketCommunication::LocalSocketCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

LocalSocketCommunication::~LocalSocketCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info LocalSocketCommunication::ImportInfoImpl(const Info& I_Info)
{
    Info imported_info;
    Receive(imported_info);
    return imported_info;
}

Info LocalSocketCommunication::ExportInfoImpl(const Info& I_Info)
{
    Send(I_Info);
    return Info(); // TODO use
}

Info LocalSocketCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    Receive(rData);
    return Info(); // TODO use
}

Info LocalSocketCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    Send(rData);
    return Info(); // TODO use
}

Info LocalSocketCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    Receive(O_ModelPart);
    return Info(); // TODO use
}

Info LocalSocketCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    Send(I_ModelPart);
    return Info(); // TODO use
}

Info LocalSocketCommunication::ConnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    using asio::local::stream_protocol;
    mpAsioSocket = std::make_shared<stream_protocol::socket>(mAsioContext);

    const fs::path bind_file_name = GetCommunicationDirectory() / "socket_bind";

    if (GetIsPrimaryConnection()) { // this is the server
        std::ofstream bind_file;
        bind_file.open(bind_file_name);
        bind_file.close();
        ::unlink(bind_file_name.c_str()); // Remove previous binding.
    }

    SynchronizeAll();

    stream_protocol::endpoint this_endpoint(bind_file_name);

    if (GetIsPrimaryConnection()) { // this is the server
        mpAsioAcceptor = std::make_shared<stream_protocol::acceptor>(mAsioContext, this_endpoint);
        SynchronizeAll();
        mpAsioAcceptor->accept(*mpAsioSocket);
        mpAsioAcceptor->close();
    } else { // this is the client
        SynchronizeAll();
        mpAsioSocket->connect(this_endpoint);
    }

    // required such that asio keeps listening for incoming messages
    mContextThread = std::thread([this]() { mAsioContext.run(); });

    return Info();

    CO_SIM_IO_CATCH
}

Info LocalSocketCommunication::DisconnectDetail(const Info& I_Info)
{
    // Request the context to close
    mAsioContext.stop();

    // Tidy up the context thread
    if (mContextThread.joinable()) mContextThread.join();

    mpAsioSocket->close();

    return Info();
}

void LocalSocketCommunication::Write(const std::string& rData)
{
    SendSize(rData.size());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()));
}

void LocalSocketCommunication::Read(std::string& rData)
{
    std::size_t received_size = ReceiveSize();
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(&(rData.front()), received_size));
}

void LocalSocketCommunication::SendSize(const std::uint64_t Size)
{
    asio::write(*mpAsioSocket, asio::buffer(&Size, sizeof(Size)));
}

std::uint64_t LocalSocketCommunication::ReceiveSize()
{
    std::uint64_t imp_size_u;
    asio::read(*mpAsioSocket, asio::buffer(&imp_size_u, sizeof(imp_size_u)));
    return imp_size_u;
}

} // namespace Internals
} // namespace CoSimIO
