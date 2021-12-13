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
        const bool IsPrimary);

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

    template<typename TDataType>
    void Send(const Internals::DataContainer<TDataType>& rData)
    {
        #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
        const std::size_t data_size = rData.size();
        std::size_t written_size=0;
        SendSize(data_size);
        const std::size_t buffer_size = GetPipeBufferSize()/sizeof(TDataType);

        std::cerr << "Size of Pipe buffer: "<< GetPipeBufferSize() << std::endl;
        std::cerr << "buffer_size: "<< buffer_size << std::endl;

        while(written_size<data_size) {
            const std::size_t data_left_to_write = data_size - written_size;
            const std::size_t current_buffer_size = data_left_to_write > buffer_size ? buffer_size : data_left_to_write;
            std::cerr <<"data_left_to_write: " << data_left_to_write << std::endl;

            const ssize_t bytes_written = write(mPipeHandleWrite, &rData[written_size], current_buffer_size*sizeof(TDataType));
            CO_SIM_IO_ERROR_IF(bytes_written < 0) << "Error in writing to Pipe!" << std::endl;

            written_size += current_buffer_size;
        }

        #endif
    }

    template<typename TDataType>
    void Receive(Internals::DataContainer<TDataType>& rData)
    {
        #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
        std::size_t received_size = ReceiveSize();
        std::size_t read_size=0;
        rData.resize(received_size);
        const std::size_t buffer_size = GetPipeBufferSize()/sizeof(TDataType);

        while(read_size<received_size) {
            const std::size_t data_left_to_read = received_size - read_size;
            const std::size_t current_buffer_size = data_left_to_read > buffer_size ? buffer_size : data_left_to_read;
            std::cerr <<"data_left_to_read: " << data_left_to_read << std::endl;

            const ssize_t bytes_written = write(mPipeHandleWrite, &rData[written_size], current_buffer_size*sizeof(TDataType));
            CO_SIM_IO_ERROR_IF(bytes_written < 0) << "Error in writing to Pipe!" << std::endl;

            read_size += current_buffer_size;
        }
        #endif
    }

    void Close();


private:
    int mPipeHandleWrite;
    int mPipeHandleRead;

    fs::path mPipeNameWrite;
    fs::path mPipeNameRead;

    void SendSize(const std::uint64_t Size);

    std::uint64_t ReceiveSize();

    std::size_t GetPipeBufferSize();
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

    void DerivedHandShake() override;
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_PIPE_COMMUNICATION_INCLUDED
