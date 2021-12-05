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

#ifndef CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED
#define CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED

// System includes
#include <chrono>

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

class CO_SIM_IO_API SocketsCommunication : public Communication
{
public:
    SocketsCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~SocketsCommunication() override;

    Info ConnectDetail(const Info& I_Info) override;

    Info DisconnectDetail(const Info& I_Info) override;

    Info ImportInfoImpl(const Info& I_Info) override;

    Info ExportInfoImpl(const Info& I_Info) override;

    Info ImportDataImpl(
        const Info& I_Info,
        Internals::DataContainer<double>& rData) override;

    Info ExportDataImpl(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData) override;

    Info ImportMeshImpl(
        const Info& I_Info,
        ModelPart& O_ModelPart) override;

    Info ExportMeshImpl(
        const Info& I_Info,
        const ModelPart& I_ModelPart) override;

private:

    asio::io_context mAsioContext;
    std::shared_ptr<asio::ip::tcp::socket> mpAsioSocket;
    std::shared_ptr<asio::ip::tcp::acceptor> mpAsioAcceptor;
    unsigned short mPortNumber=0;
    std::vector<int> mAllPortNumbers;
    std::thread mContextThread;


    std::string GetCommunicationName() const override {return "sockets";}

    void PrepareConnection(const Info& I_Info) override;

    Info GetCommunicationSettings() const override;

    void GetPortNumber();

    void Write(const std::string& rData);

    void Read(std::string& rData);

    template<class TObjectType>
    void Send(const TObjectType& rObject)
    {
        StreamSerializer serializer;
        serializer.save("object", rObject);

        Write(serializer.GetStringRepresentation());
    }

    template<class TObjectType>
    void Receive(TObjectType& rObject)
    {
        std::string buffer;
        Read(buffer);
        StreamSerializer serializer(buffer);
        serializer.load("object", rObject);
    }

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED
