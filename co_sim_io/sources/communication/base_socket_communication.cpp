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
#ifdef CO_SIM_IO_TIMESTAMP_DEBUG
#include <chrono>
#include <ctime>
#include <iomanip>
#endif

// Project includes
#include "includes/communication/base_socket_communication.hpp"

namespace CoSimIO {
namespace Internals {

template<class TSocketType>
BaseSocketCommunication<TSocketType>::~BaseSocketCommunication()
{
    CO_SIM_IO_TRY

    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }

    CO_SIM_IO_CATCH
}

template<class TSocketType>
Info BaseSocketCommunication<TSocketType>::ConnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    DebugTimeInfo("ConnectDetail", "Before connect");

    // required such that asio keeps listening for incoming messages
    mContextThread = std::thread([this]() { mAsioContext.run(); });

    DebugTimeInfo("ConnectDetail", "After connect");

    return Info();

    CO_SIM_IO_CATCH
}

template<class TSocketType>
Info BaseSocketCommunication<TSocketType>::DisconnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    DebugTimeInfo("DisconnectDetail", "Before disconnect");

    // Request the context to close
    mAsioContext.stop();

    // Tidy up the context thread
    if (mContextThread.joinable()) mContextThread.join();

    mpAsioSocket->close();
    mpAsioSocket.reset(); // important to release the resources (otherwise crashes in Win with release compilation)

    DebugTimeInfo("DisconnectDetail", "After disconnect");

    return Info();

    CO_SIM_IO_CATCH
}

template<class TSocketType>
double BaseSocketCommunication<TSocketType>::SendString(
    const Info& I_Info,
    const std::string& rData)
{
    CO_SIM_IO_TRY

    SendSize(rData.size()); // serves also as synchronization for time measurement

    DebugTimeInfo("SendString", "Before send");

    const auto start_time(std::chrono::steady_clock::now());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()));

    DebugTimeInfo("SendString", "After send");

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

template<class TSocketType>
double BaseSocketCommunication<TSocketType>::ReceiveString(
    const Info& I_Info,
    std::string& rData)
{
    CO_SIM_IO_TRY

    std::size_t received_size = ReceiveSize(); // serves also as synchronization for time measurement

    DebugTimeInfo("ReceiveString", "Before receive");

    const auto start_time(std::chrono::steady_clock::now());
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(&(rData.front()), received_size));

    DebugTimeInfo("ReceiveString", "After receive");

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

template<class TSocketType>
double BaseSocketCommunication<TSocketType>::SendDataContainer(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    DebugTimeInfo("SendDataContainer", "Before send");

    SendSize(rData.size()); // serves also as synchronization for time measurement

    const auto start_time(std::chrono::steady_clock::now());
    asio::write(*mpAsioSocket, asio::buffer(rData.data(), rData.size()*sizeof(double)));

    DebugTimeInfo("SendDataContainer", "After send");

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

template<class TSocketType>
double BaseSocketCommunication<TSocketType>::ReceiveDataContainer(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    DebugTimeInfo("ReceiveDataContainer", "Before receive");

    std::size_t received_size = ReceiveSize(); // serves also as synchronization for time measurement

    const auto start_time(std::chrono::steady_clock::now());
    rData.resize(received_size);
    asio::read(*mpAsioSocket, asio::buffer(rData.data(), rData.size()*sizeof(double)));

    DebugTimeInfo("ReceiveDataContainer", "After receive");

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

template<class TSocketType>
void BaseSocketCommunication<TSocketType>::SendSize(const std::uint64_t Size)
{
    CO_SIM_IO_TRY

    DebugTimeInfo("SendSize", "Before send");
#ifdef CO_SIM_IO_TIMESTAMP_DEBUG
    CO_SIM_IO_INFO("CoSimIO::SendSize") << Size << std::endl;
#endif

    asio::write(*mpAsioSocket, asio::buffer(&Size, sizeof(Size)));

    DebugTimeInfo("SendSize", "After send");

    CO_SIM_IO_CATCH
}

template<class TSocketType>
std::uint64_t BaseSocketCommunication<TSocketType>::ReceiveSize()
{
    CO_SIM_IO_TRY

    DebugTimeInfo("ReceiveSize", "Before receive");

    std::uint64_t imp_size_u;
    asio::read(*mpAsioSocket, asio::buffer(&imp_size_u, sizeof(imp_size_u)));

    DebugTimeInfo("ReceiveSize", "After receive");
#ifdef CO_SIM_IO_TIMESTAMP_DEBUG
    CO_SIM_IO_INFO("CoSimIO::ReceiveSize") << imp_size_u << std::endl;
#endif

    return imp_size_u;

    CO_SIM_IO_CATCH
}

template<class TSocketType>
void BaseSocketCommunication<TSocketType>::DebugTimeInfo(
    const std::string& rLabel,
    const std::string& rMessage
    ) const
{
#ifdef CO_SIM_IO_TIMESTAMP_DEBUG
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Convert to time_t to get calendar time
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to local time
    std::tm local_time = *std::localtime(&now_time);

    // Convert to calendar time to get localtime
    CO_SIM_IO_INFO("CoSimIO::" + rLabel) << rMessage << " time: " << std::put_time(&local_time, "%H:%M:%S") << std::endl;
#endif
}

template class BaseSocketCommunication<asio::ip::tcp::socket>;
template class BaseSocketCommunication<asio::local::stream_protocol::socket>;

} // namespace Internals
} // namespace CoSimIO
