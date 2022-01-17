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

#ifndef CO_SIM_IO_LOCAL_SOCKETS_COMMUNICATION_INCLUDED
#define CO_SIM_IO_LOCAL_SOCKETS_COMMUNICATION_INCLUDED

// System includes
#include <thread>

// Project includes
#include "communication.hpp"

// External includes
#define ASIO_NO_DEPRECATED // disabling deprecated features/interfaces
#define ASIO_STANDALONE // independent of boost
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601 // see "https://github.com/chriskohlhoff/asio/issues/596"
#endif
#include "asio.hpp"

namespace CoSimIO {
namespace Internals {

class CO_SIM_IO_API LocalSocketCommunication : public Communication
{
public:
    LocalSocketCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~LocalSocketCommunication() override;

    Info ConnectDetail(const Info& I_Info) override;

    Info DisconnectDetail(const Info& I_Info) override;

private:

    asio::io_context mAsioContext;
    std::shared_ptr<asio::local::stream_protocol::socket> mpAsioSocket;
    std::shared_ptr<asio::local::stream_protocol::acceptor> mpAsioAcceptor;
    std::thread mContextThread;


    std::string GetCommunicationName() const override {return "local_socket";}

    double SendString(const std::string& rData) override;

    double ReceiveString(std::string& rData) override;

    double SendDataContainer(const Internals::DataContainer<double>& rData) override;

    double ReceiveDataContainer(Internals::DataContainer<double>& rData) override;

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_LOCAL_SOCKETS_COMMUNICATION_INCLUDED
