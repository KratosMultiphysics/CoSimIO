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
    using namespace asio::ip;

    mpAsioSocket = std::make_shared<asio::ip::tcp::socket>(mAsioContext);
    asio::ip::tcp::endpoint local_endpoint(asio::ip::make_address("127.0.0.1"), 60000);

    if (GetIsPrimaryConnection()) { // this is the server
        std::cerr << "SERVER DEBUG 0" << std::endl;
        try {
            tcp::acceptor acceptor(mAsioContext, local_endpoint);

            std::cerr << "SERVER DEBUG 1" << std::endl;

            std::cerr << "SERVER DEBUG 2" << std::endl;
            SynchronizeAll();
            acceptor.accept(*mpAsioSocket);


            std::cerr << "SERVER DEBUG 3" << std::endl;
            std::string message = "make_daytime_string()\n";

            asio::error_code ignored_error;
            asio::write(*mpAsioSocket, asio::buffer(message), ignored_error);
        } catch (std::exception& e) {
            std::cerr << "Serveer error:" << e.what() << std::endl;
        }
    } else { // this is the client
        std::cerr << "CLIENT DEBUG 0" << std::endl;
        try {
            SynchronizeAll();
            tcp::resolver resolver(mAsioContext);
            std::cerr << "CLIENT DEBUG 1" << std::endl;
            // tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "60000");
            // std::cerr << "CLIENT DEBUG 2" << std::endl;

            mpAsioSocket->connect(local_endpoint);

            // asio::connect(*mpAsioSocket, local_endpoint);
            std::cerr << "CLIENT DEBUG 2" << std::endl;

            asio::error_code error;

            std::cerr << "CLIENT DEBUG 3" << std::endl;
            size_t len = mpAsioSocket->read_some(asio::buffer(StBuffer), error);
            std::cerr << "CLIENT DEBUG 4" << std::endl;

            std::cerr << "CLIENT DEBUG 5" << std::endl;
            std::cout.write(StBuffer.data(), len);
            std::cerr << "CLIENT DEBUG 6" << std::endl;
        } catch (std::exception& e) {
            std::cerr <<"Client error: " <<e.what() << std::endl;
        }
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

} // namespace Internals
} // namespace CoSimIO
