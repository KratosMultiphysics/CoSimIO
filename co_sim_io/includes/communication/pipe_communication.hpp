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

#ifndef CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
#define CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED

// System includes
#include <unordered_map>

// Project includes
#include "communication.hpp"

namespace CoSimIO {
namespace Internals {

class CO_SIM_IO_API PipeCommunication : public Communication
{
public:
    PipeCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~PipeCommunication() override;

private:
class BidirectionalPipe
{
public:
    BidirectionalPipe(
        const fs::path& rPipeDir,
        const fs::path& rBasePipeName,
        const bool IsPrimary,
        const std::size_t BufferSize);

    template<class TObjectType>
    double Send(const TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer;
        serializer.save("object", rObject);
        const double elapsed_time_save = Utilities::ElapsedSeconds(start_time);

        const double elapsed_time_write = Write(serializer.GetStringRepresentation(), 1);

        return elapsed_time_save + elapsed_time_write;

        CO_SIM_IO_CATCH
    }

    template<class TObjectType>
    double Receive(TObjectType& rObject)
    {
        CO_SIM_IO_TRY

        std::string buffer;
        const double elapsed_time_read = Read(buffer, 1);

        const auto start_time(std::chrono::steady_clock::now());
        StreamSerializer serializer(buffer);
        serializer.load("object", rObject);
        const double elapsed_time_load = Utilities::ElapsedSeconds(start_time);

        return elapsed_time_read + elapsed_time_load;

        CO_SIM_IO_CATCH
    }

    template<typename TDataType>
    double Send(const Internals::DataContainer<TDataType>& rData)
    {
        return Write(rData, sizeof(TDataType));
    }

    template<typename TDataType>
    double Receive(Internals::DataContainer<TDataType>& rData)
    {
        return Read(rData, sizeof(TDataType));
    }

    template<typename TDataType>
    double Write(const TDataType& rData, const std::size_t SizeDataType)
    {
        CO_SIM_IO_TRY

        #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
        const std::size_t data_size = rData.size();
        std::size_t written_size=0;
        SendSize(data_size); // serves also as synchronization for time measurement

        const auto start_time(std::chrono::steady_clock::now());
        const std::size_t buffer_size = mBufferSize/SizeDataType;

        while(written_size<data_size) {
            const std::size_t data_left_to_write = data_size - written_size;
            const std::size_t current_buffer_size = data_left_to_write > buffer_size ? buffer_size : data_left_to_write;

            const ssize_t bytes_written = write(mPipeHandleWrite, &rData[written_size], current_buffer_size*SizeDataType);
            CO_SIM_IO_ERROR_IF(bytes_written < 0) << "Error in writing to Pipe!" << std::endl;

            written_size += current_buffer_size;
        }
        return Utilities::ElapsedSeconds(start_time);
        #else
        return 0.0;

        #endif

        CO_SIM_IO_CATCH
    }

    template<typename TDataType>
    double Read(TDataType& rData, const std::size_t SizeDataType)
    {
        CO_SIM_IO_TRY

        #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
        std::size_t received_size = ReceiveSize(); // serves also as synchronization for time measurement
        std::size_t read_size=0;

        const auto start_time(std::chrono::steady_clock::now());
        rData.resize(received_size);
        const std::size_t buffer_size = mBufferSize/SizeDataType;

        while(read_size<received_size) {
            const std::size_t data_left_to_read = received_size - read_size;
            const std::size_t current_buffer_size = data_left_to_read > buffer_size ? buffer_size : data_left_to_read;

            const ssize_t bytes_written = read(mPipeHandleRead, &rData[read_size], current_buffer_size*SizeDataType);
            CO_SIM_IO_ERROR_IF(bytes_written < 0) << "Error in reading from Pipe!" << std::endl;

            read_size += current_buffer_size;
        }
        return Utilities::ElapsedSeconds(start_time);
        #else
        return 0.0;

        #endif

        CO_SIM_IO_CATCH
    }

    void Close();


private:
    int mPipeHandleWrite;
    int mPipeHandleRead;

    fs::path mPipeNameWrite;
    fs::path mPipeNameRead;

    const std::size_t mBufferSize;

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();
};

    std::shared_ptr<BidirectionalPipe> mpPipe;

    std::string GetCommunicationName() const override {return "pipe";}

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

    void DerivedHandShake() const override;

    std::size_t GetPipeBufferSize(const Info& I_Info) const;
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
