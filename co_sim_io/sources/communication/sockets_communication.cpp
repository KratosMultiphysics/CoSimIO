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
#include <stdexcept>

// Project includes
#include "includes/communication/sockets_communication.hpp"

namespace CoSimIO {
namespace Internals {

std::vector<char> StBuffer(20*1024);

void GrabSomeData(asio::ip::tcp::socket& rSocket)
{
    rSocket.async_read_some(asio::buffer(StBuffer.data(), StBuffer.size()),
        [&](std::error_code ec, std::size_t length){
            if (!ec) {
                std::cout << "\n\nRead " << length << " bytes\n\n";
                for(std::size_t i=0;i<length;++i) {
                    std::cout << StBuffer[i];
                }
                GrabSomeData(rSocket);
            }
        });
}

SocketsCommunication::SocketsCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
    // std::error_code ec;

    // asio::mAsioContext context;

    // asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
    // asio::ip::tcp::endpoint endpoint_2(asio::ip::make_address("51.38.81.49", ec), 80);
    // asio::ip::tcp::endpoint local_endpoint(asio::ip::make_address("127.0.0.1", ec), 80);

    // asio::ip::tcp::socket socket(context);

    // socket.connect(endpoint_2, ec);

    // if (!ec) {
    //     std::cout << "ASIO connected" << std::endl;
    // } else {
    //     std::cout << "ASIO connection ffdaile with error code:\n" << ec.message() << std::endl;
    // }

    // if (socket.is_open()) {
    //     GrabSomeData(socket);

    //     std::string str_request =
    //         "GET /index.html HTTP/1.1\r\n"
    //         "Host: example.com\r\n"
    //         "Connection: close\r\n\r\n";

    //     socket.write_some(asio::buffer(str_request.data(), str_request.size()), ec);


    //     using namespace std::chrono;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));


    // }


}

SocketsCommunication::~SocketsCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info SocketsCommunication::ConnectDetail(const Info& I_Info)
{
    if (!GetIsPrimaryConnection()) {GetPortNumber();}

    using namespace asio::ip;

    mpAsioSocket = std::make_shared<asio::ip::tcp::socket>(mAsioContext);
    if (GetIsPrimaryConnection()) { // this is the server
        mpAsioAcceptor->accept(*mpAsioSocket);
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
    }
}

Info SocketsCommunication::GetCommunicationSettings() const
{
    Info info;
    std::string port_numbers = std::to_string(mPortNumber);
    info.Set("port_numbers", port_numbers);
    return info;
}

void SocketsCommunication::GetPortNumber()
{
    CO_SIM_IO_ERROR_IF(GetIsPrimaryConnection()) << "This function can only be used as secondary connection!" << std::endl;

    const auto partner_info = GetPartnerInfo();
    const std::string ports = partner_info.Get<Info>("communication_settings").Get<std::string>("port_numbers");

    mPortNumber = std::stoul(ports);

    CO_SIM_IO_INFO_IF("CoSimIO", GetEchoLevel()>1) << "Using port number " << mPortNumber << std::endl;
}

} // namespace Internals
} // namespace CoSimIO
