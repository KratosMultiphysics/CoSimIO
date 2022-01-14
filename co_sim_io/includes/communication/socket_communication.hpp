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

class CO_SIM_IO_API SocketCommunication : public Communication
{
public:
    SocketCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~SocketCommunication() override;

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
    std::string mIpAddress;

    std::string GetCommunicationName() const override {return "socket";}

    void PrepareConnection(const Info& I_Info) override;

    void DerivedHandShake() const override;

    Info GetCommunicationSettings() const override;

    void GetPortNumber();

    double Write(const std::string& rData);

    double Read(std::string& rData);

    template<class TObjectType>
    double Send(const TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer;
        serializer.save("object", rObject);
        const double elapsed_time_save = Utilities::ElapsedSeconds(start_time);

        const double elapsed_time_write = Write(serializer.GetStringRepresentation());
        return elapsed_time_save + elapsed_time_write;

        CO_SIM_IO_CATCH
    }

    template<class TObjectType>
    double Receive(TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        std::string buffer;
        const double elapsed_time_read = Read(buffer);

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer(buffer);
        serializer.load("object", rObject);
        const double elapsed_time_load = Utilities::ElapsedSeconds(start_time);

        return elapsed_time_read+elapsed_time_load;

        CO_SIM_IO_CATCH
    }

    template<typename TDataType>
    double Send(const Internals::DataContainer<TDataType>& rData)
    {
        CO_SIM_IO_TRY

        SendSize(rData.size()); // serves also as synchronization for time measurement

        const auto start_time(std::chrono::steady_clock::now());
        asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()*sizeof(TDataType)));
        return Utilities::ElapsedSeconds(start_time);

        CO_SIM_IO_CATCH
    }

    template<typename TDataType>
    double Receive(Internals::DataContainer<TDataType>& rData)
    {
        CO_SIM_IO_TRY

        std::size_t received_size = ReceiveSize(); // serves also as synchronization for time measurement

        const auto start_time(std::chrono::steady_clock::now());
        rData.resize(received_size);
        asio::read(*mpAsioSocket, asio::buffer(rData.data(), received_size*sizeof(TDataType)));
        return Utilities::ElapsedSeconds(start_time);

        CO_SIM_IO_CATCH
    }

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_SOCKETS_COMMUNICATION_INCLUDED
