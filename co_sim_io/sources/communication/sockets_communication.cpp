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

// External includes
#define ASIO_NO_DEPRECATED // disabling deprecated features/interfaces
#define ASIO_STANDALONE // independent of boost
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601 // see "https://github.com/chriskohlhoff/asio/issues/596"
#endif
#include "asio.hpp"

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
    std::error_code ec;

    asio::io_context context;

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);
    asio::ip::tcp::endpoint endpoint_2(asio::ip::make_address("51.38.81.49", ec), 80);
    asio::ip::tcp::endpoint local_endpoint(asio::ip::make_address("127.0.0.1", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint_2, ec);

    if (!ec) {
        std::cout << "ASIO connected" << std::endl;
    } else {
        std::cout << "ASIO connection ffdaile with error code:\n" << ec.message() << std::endl;
    }

    if (socket.is_open()) {
        std::string str_request =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(str_request.data(), str_request.size()), ec);

        socket.wait(socket.wait_read);

        std::size_t bytes = socket.available();

        std::cout << "Bytes available: " << bytes << std::endl;

        if (bytes > 0) {
            std::vector<char> v_buffer(bytes);
            socket.read_some(asio::buffer(v_buffer.data(), v_buffer.size()), ec);

            for (auto c : v_buffer) {
                std::cout << c;
            }
        }


    }


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
    return Info();
}

Info SocketsCommunication::DisconnectDetail(const Info& I_Info)
{
    return Info();
}

} // namespace Internals
} // namespace CoSimIO
