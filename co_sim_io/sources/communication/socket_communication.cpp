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
    std::istringstream ss(rString);
    std::string token;

    std::vector<std::string> splitted_string;
    while(std::getline(ss, token, Delimiter)) {
        splitted_string.push_back(token);
    }

    return splitted_string;
}

} // helpers namespace

SocketCommunication::SocketCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

SocketCommunication::~SocketCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info SocketCommunication::ImportInfoImpl(const Info& I_Info)
{
    Info imported_info;
    Receive(imported_info);
    return imported_info;
}

Info SocketCommunication::ExportInfoImpl(const Info& I_Info)
{
    Send(I_Info);
    return Info(); // TODO use
}

Info SocketCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    Receive(rData);
    return Info(); // TODO use
}

Info SocketCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    Send(rData);
    return Info(); // TODO use
}

Info SocketCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    Receive(O_ModelPart);
    return Info(); // TODO use
}

Info SocketCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    Send(I_ModelPart);
    return Info(); // TODO use
}

Info SocketCommunication::ConnectDetail(const Info& I_Info)
{
    if (!GetIsPrimaryConnection()) {GetPortNumber();}

    using namespace asio::ip;

    mpAsioSocket = std::make_shared<asio::ip::tcp::socket>(mAsioContext);
    if (GetIsPrimaryConnection()) { // this is the server
        mpAsioAcceptor->accept(*mpAsioSocket);
        mpAsioAcceptor->close();
    } else { // this is the client
        tcp::endpoint my_endpoint(asio::ip::make_address(GetIpAddress()), mPortNumber);
        mpAsioSocket->connect(my_endpoint);
    }

    // required such that asio keeps listening for incoming messages
    mContextThread = std::thread([this]() { mAsioContext.run(); });

    return Info();
}

Info SocketCommunication::DisconnectDetail(const Info& I_Info)
{
    // Request the context to close
    mAsioContext.stop();

    // Tidy up the context thread
    if (mContextThread.joinable()) mContextThread.join();

    mpAsioSocket->close();

    return Info();
}


void SocketCommunication::PrepareConnection(const Info& I_Info)
{
    // preparing the acceptors to get the ports used for connecting the sockets
    if (GetIsPrimaryConnection()) {
        using namespace asio::ip;
        tcp::endpoint port_selection_endpoint(asio::ip::make_address(GetIpAddress()), 0); // using port 0 means that it will look for a free port
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

void SocketCommunication::DerivedHandShake() const
{
}

Info SocketCommunication::GetCommunicationSettings() const
{
    Info info;

    if (GetIsPrimaryConnection() && GetDataCommunicator().Rank() == 0) {
        std::stringstream port_numbers_stream;
        std::copy(mAllPortNumbers.begin(), mAllPortNumbers.end(), std::ostream_iterator<int>(port_numbers_stream, "-"));
        info.Set("port_numbers", port_numbers_stream.str());
    }

    return info;
}

void SocketCommunication::GetPortNumber()
{
    CO_SIM_IO_ERROR_IF(GetIsPrimaryConnection()) << "This function can only be used as secondary connection!" << std::endl;

    const auto partner_info = GetPartnerInfo();
    const std::string ports_string = partner_info.Get<Info>("communication_settings").Get<std::string>("port_numbers");

    std::vector<std::string> ports = SplitStringByDelimiter(ports_string, '-');

    CO_SIM_IO_ERROR_IF(static_cast<int>(ports.size()) != GetDataCommunicator().Size()) << "Wrong number of ports!" << std::endl;

    mPortNumber = static_cast<unsigned short>(std::stoul(ports[GetDataCommunicator().Rank()]));

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number " << mPortNumber << std::endl;
}

void SocketCommunication::Write(const std::string& rData)
{
    SendSize(rData.size());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()));
}

void SocketCommunication::Read(std::string& rData)
{
    std::size_t received_size = ReceiveSize();
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(&(rData.front()), received_size));
}

void SocketCommunication::SendSize(const std::uint64_t Size)
{
    asio::write(*mpAsioSocket, asio::buffer(&Size, sizeof(Size)));
}

std::uint64_t SocketCommunication::ReceiveSize()
{
    std::uint64_t imp_size_u;
    asio::read(*mpAsioSocket, asio::buffer(&imp_size_u, sizeof(imp_size_u)));
    return imp_size_u;
}

std::string SocketCommunication::GetIpAddress() const
{
    return "127.0.0.1";
}

} // namespace Internals
} // namespace CoSimIO
