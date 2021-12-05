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
#include <stdexcept>

// Project includes
#include "includes/communication/sockets_communication.hpp"

namespace CoSimIO {
namespace Internals {

namespace {

std::vector<std::string> SplitStringByDelimiter(
const std::string& rString,
const char Delimiter)
{
    std::istringstream ss(rString);
    std::string token;

    std::vector<std::string> splitted_string;
    while(std::getline(ss, token, Delimiter)) {
        splitted_string.push_back(token);
    }

    return splitted_string;
}
}

std::vector<char> StBuffer(20*1024);


SocketsCommunication::SocketsCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

SocketsCommunication::~SocketsCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info SocketsCommunication::ImportInfoImpl(const Info& I_Info)
{
    Info imported_info;
    Receive(imported_info);
    return imported_info;
}

Info SocketsCommunication::ExportInfoImpl(const Info& I_Info)
{
    Send(I_Info);
    return Info(); // TODO use
}

Info SocketsCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    Receive(rData);
    return Info(); // TODO use
}

Info SocketsCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    Send(rData);
    return Info(); // TODO use
}

Info SocketsCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    Receive(O_ModelPart);
    return Info(); // TODO use
}

Info SocketsCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    Send(I_ModelPart);
    return Info(); // TODO use
}

Info SocketsCommunication::ConnectDetail(const Info& I_Info)
{
    if (!GetIsPrimaryConnection()) {GetPortNumber();}

    using namespace asio::ip;

    mpAsioSocket = std::make_shared<asio::ip::tcp::socket>(mAsioContext);
    if (GetIsPrimaryConnection()) { // this is the server
        mpAsioAcceptor->accept(*mpAsioSocket);
        mpAsioAcceptor->close();
        asio::error_code ignored_error;
        std::string message = "make_daytime_string()\n";
        asio::write(*mpAsioSocket, asio::buffer(message), ignored_error);
    } else { // this is the client
        tcp::endpoint my_endpoint(asio::ip::make_address("127.0.0.1"), mPortNumber);

        mpAsioSocket->connect(my_endpoint);
        asio::error_code error;
        size_t len = mpAsioSocket->read_some(asio::buffer(StBuffer), error);
        std::cout.write(StBuffer.data(), len);
    }

    mContextThread = std::thread([this]() { mAsioContext.run(); });

    return Info();
}

Info SocketsCommunication::DisconnectDetail(const Info& I_Info)
{
    // Request the context to close
    mAsioContext.stop();

    // Tidy up the context thread
    if (mContextThread.joinable()) mContextThread.join();

    mpAsioSocket->close();

    return Info();
}


void SocketsCommunication::PrepareConnection(const Info& I_Info)
{
    // preparing the acceptors to get the ports used for connecting the sockets
    if (GetIsPrimaryConnection()) {
        using namespace asio::ip;
        tcp::endpoint port_selection_endpoint(asio::ip::make_address("127.0.0.1"), 0);
        mpAsioAcceptor = std::make_shared<tcp::acceptor>(mAsioContext, port_selection_endpoint);
        mPortNumber = mpAsioAcceptor->local_endpoint().port();

        CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number " << mPortNumber << std::endl;

        // collect all port numbers on rank 0
        // to exchange them during the handshake
        // (which happens only on rank 0)
        const auto& r_data_comm = GetDataCommunicator();
        mAllPortNumbers.resize(r_data_comm.Size());
        std::vector<int> my_ports(1);
        my_ports[0] = mPortNumber;
        r_data_comm.Gather(my_ports, mAllPortNumbers, 0);
    }
}

Info SocketsCommunication::GetCommunicationSettings() const
{
    Info info;

    if (GetIsPrimaryConnection() && GetDataCommunicator().Rank() == 0) {
        std::stringstream port_numbers_stream;
        std::copy(mAllPortNumbers.begin(), mAllPortNumbers.end(), std::ostream_iterator<int>(port_numbers_stream, "-"));
        info.Set("port_numbers", port_numbers_stream.str());
    }

    return info;
}

void SocketsCommunication::GetPortNumber()
{
    CO_SIM_IO_ERROR_IF(GetIsPrimaryConnection()) << "This function can only be used as secondary connection!" << std::endl;

    const auto partner_info = GetPartnerInfo();
    const std::string ports_string = partner_info.Get<Info>("communication_settings").Get<std::string>("port_numbers");

    std::vector<std::string> ports = SplitStringByDelimiter(ports_string, '-');

    CO_SIM_IO_ERROR_IF(static_cast<int>(ports.size()) != GetDataCommunicator().Size()) << "Wrong number of ports!" << std::endl;

    mPortNumber = std::stoul(ports[GetDataCommunicator().Rank()]);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number " << mPortNumber << std::endl;
}

void SocketsCommunication::Write(const std::string& rData)
{
    SendSize(rData.size());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()));
}

void SocketsCommunication::Read(std::string& rData)
{
    std::size_t received_size = ReceiveSize();
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(&(rData.front()), received_size));
}

void SocketsCommunication::SendSize(const std::uint64_t Size)
{
    asio::write(*mpAsioSocket, asio::buffer(&Size, sizeof(Size)));
}

std::uint64_t SocketsCommunication::ReceiveSize()
{
    std::uint64_t imp_size_u;
    asio::read(*mpAsioSocket, asio::buffer(&imp_size_u, sizeof(imp_size_u)));
    return imp_size_u;
}

} // namespace Internals
} // namespace CoSimIO
