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
#include "includes/communication/socket_communication.hpp"

namespace CoSimIO {
namespace Internals {

namespace {

std::vector<std::string> SplitStringByDelimiter(
const std::string& rString,
const char Delimiter)
{
    CO_SIM_IO_TRY

    std::istringstream ss(rString);
    std::string token;

    std::vector<std::string> splitted_string;
    while(std::getline(ss, token, Delimiter)) {
        splitted_string.push_back(token);
    }

    return splitted_string;

    CO_SIM_IO_CATCH
}

std::string GetIpAddress(const Info& I_Settings)
{
    CO_SIM_IO_TRY

    if (I_Settings.Has("ip_address")) {
        return I_Settings.Get<std::string>("ip_address");
    } else if (I_Settings.Has("network_name")) {
        return "127.0.0.1";
    } else {
        return "127.0.0.1"; // local loopback interface
    }

    CO_SIM_IO_CATCH
}

} // helpers namespace

SocketCommunication::SocketCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
    CO_SIM_IO_TRY

    mIpAddress = GetIpAddress(I_Settings);

    CO_SIM_IO_CATCH
}

SocketCommunication::~SocketCommunication()
{
    CO_SIM_IO_TRY

    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ImportInfoImpl(const Info& I_Info)
{
    CO_SIM_IO_TRY

    Info imported_info;
    Receive(imported_info);
    return imported_info;

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ExportInfoImpl(const Info& I_Info)
{
    CO_SIM_IO_TRY

    Send(I_Info);
    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    Receive(rData);
    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    Send(rData);
    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    CO_SIM_IO_TRY

    Receive(O_ModelPart);
    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    CO_SIM_IO_TRY

    Send(I_ModelPart);
    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info SocketCommunication::ConnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    if (!GetIsPrimaryConnection()) {GetPortNumber();}

    using namespace asio::ip;

    mpAsioSocket = std::make_shared<asio::ip::tcp::socket>(mAsioContext);
    if (GetIsPrimaryConnection()) { // this is the server
        mpAsioAcceptor->accept(*mpAsioSocket);
        mpAsioAcceptor->close();
    } else { // this is the client
        tcp::endpoint my_endpoint(asio::ip::make_address(mIpAddress), mPortNumber);
        mpAsioSocket->connect(my_endpoint);
    }

    // required such that asio keeps listening for incoming messages
    mContextThread = std::thread([this]() { mAsioContext.run(); });

    return Info();

    CO_SIM_IO_CATCH
}

Info SocketCommunication::DisconnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    // Request the context to close
    mAsioContext.stop();

    // Tidy up the context thread
    if (mContextThread.joinable()) mContextThread.join();

    mpAsioSocket->close();

    return Info();

    CO_SIM_IO_CATCH
}


void SocketCommunication::PrepareConnection(const Info& I_Info)
{
    CO_SIM_IO_TRY

    // CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>0) << "Using IP Address: " << GetIpAddress(true) << std::endl;

    // preparing the acceptors to get the ports used for connecting the sockets
    if (GetIsPrimaryConnection()) {
        using namespace asio::ip;
        tcp::endpoint port_selection_endpoint(asio::ip::make_address(mIpAddress), 0); // using port 0 means that it will look for a free port
        mpAsioAcceptor = std::make_shared<tcp::acceptor>(mAsioContext, port_selection_endpoint);
        mPortNumber = mpAsioAcceptor->local_endpoint().port();

        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number: " << mPortNumber << std::endl;

        // collect all port numbers on rank 0
        // to exchange them during the handshake
        // (which happens only on rank 0)
        const auto& r_data_comm = GetDataCommunicator();
        mAllPortNumbers.resize(r_data_comm.Size());
        std::vector<int> my_ports(1);
        my_ports[0] = mPortNumber;
        r_data_comm.Gather(my_ports, mAllPortNumbers, 0);
    }

    CO_SIM_IO_CATCH
}

void SocketCommunication::DerivedHandShake() const
{
    CO_SIM_IO_TRY

    CO_SIM_IO_ERROR_IF(GetMyInfo().Get<Info>("communication_settings").Get<std::string>("ip_address") != GetPartnerInfo().Get<Info>("communication_settings").Get<std::string>("ip_address")) << "IP addresses don't match!" << std::endl;

    CO_SIM_IO_CATCH
}

Info SocketCommunication::GetCommunicationSettings() const
{
    CO_SIM_IO_TRY

    Info info;
    info.Set("ip_address", mIpAddress);

    if (GetIsPrimaryConnection() && GetDataCommunicator().Rank() == 0) {
        std::stringstream port_numbers_stream;
        std::copy(mAllPortNumbers.begin(), mAllPortNumbers.end(), std::ostream_iterator<int>(port_numbers_stream, "-"));
        info.Set("port_numbers", port_numbers_stream.str());
    }

    return info;

    CO_SIM_IO_CATCH
}

void SocketCommunication::GetPortNumber()
{
    CO_SIM_IO_TRY

    CO_SIM_IO_ERROR_IF(GetIsPrimaryConnection()) << "This function can only be used as secondary connection!" << std::endl;

    const auto partner_info = GetPartnerInfo();
    const std::string ports_string = partner_info.Get<Info>("communication_settings").Get<std::string>("port_numbers");

    std::vector<std::string> ports = SplitStringByDelimiter(ports_string, '-');

    CO_SIM_IO_ERROR_IF(static_cast<int>(ports.size()) != GetDataCommunicator().Size()) << "Wrong number of ports!" << std::endl;

    mPortNumber = static_cast<unsigned short>(std::stoul(ports[GetDataCommunicator().Rank()]));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number: " << mPortNumber << std::endl;

    CO_SIM_IO_CATCH
}

void SocketCommunication::Write(const std::string& rData)
{
    CO_SIM_IO_TRY

    SendSize(rData.size());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()));

    CO_SIM_IO_CATCH
}

void SocketCommunication::Read(std::string& rData)
{
    CO_SIM_IO_TRY

    std::size_t received_size = ReceiveSize();
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(&(rData.front()), received_size));

    CO_SIM_IO_CATCH
}

void SocketCommunication::SendSize(const std::uint64_t Size)
{
    CO_SIM_IO_TRY

    asio::write(*mpAsioSocket, asio::buffer(&Size, sizeof(Size)));

    CO_SIM_IO_CATCH
}

std::uint64_t SocketCommunication::ReceiveSize()
{
    CO_SIM_IO_TRY

    std::uint64_t imp_size_u;
    asio::read(*mpAsioSocket, asio::buffer(&imp_size_u, sizeof(imp_size_u)));
    return imp_size_u;

    CO_SIM_IO_CATCH
}

} // namespace Internals
} // namespace CoSimIO
