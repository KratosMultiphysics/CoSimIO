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
#include <utility> // std::pair

// Project includes
#include "communication.hpp"
#include "includes/utilities.hpp"
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>

namespace CoSimIO {
namespace Internals {

class BidirectionalPipeUnix
{
public:
    BidirectionalPipeUnix(const fs::path& rPipeDir, const fs::path& rBasePipeName, const bool IsPrimary)
    {
        mPipeNameWrite = rPipeDir / rBasePipeName;
        mPipeNameRead  = rPipeDir / rBasePipeName;

        if (IsPrimary) {
            mPipeNameWrite += "_p2s";
            mPipeNameRead  += "_s2p";

            CO_SIM_IO_ERROR_IF(mkfifo(mPipeNameWrite.c_str(), 0666) != 0) << "Pipe " << mPipeNameWrite << " could not be created!" << std::endl;
            CO_SIM_IO_ERROR_IF(mkfifo(mPipeNameRead.c_str(), 0666) != 0) << "Pipe " << mPipeNameRead << " could not be created!" << std::endl;
            CO_SIM_IO_ERROR_IF((mPipeHandleWrite = open(mPipeNameWrite.c_str(), O_WRONLY)) < 0) << "Pipe " << mPipeNameWrite << " could not be opened!" << std::endl;
            CO_SIM_IO_ERROR_IF((mPipeHandleRead = open(mPipeNameRead.c_str(), O_RDONLY)) < 0) << "Pipe " << mPipeNameRead << " could not be opened!" << std::endl;

        } else {
            mPipeNameWrite += "_s2p";
            mPipeNameRead  += "_p2s";

            WaitUntilPathExists(mPipeNameWrite); // written last hence wait for it

            CO_SIM_IO_ERROR_IF((mPipeHandleRead = open(mPipeNameRead.c_str(), O_RDONLY)) < 0) << "Pipe " << mPipeNameRead << " could not be opened!" << std::endl;
            CO_SIM_IO_ERROR_IF((mPipeHandleWrite = open(mPipeNameWrite.c_str(), O_WRONLY)) < 0) << "Pipe " << mPipeNameWrite << " could not be opened!" << std::endl;
        }
    }

    void Write(const std::string& rData)
    {
        SendSize(rData.size());
        write(mPipeHandleWrite, rData.c_str(), rData.size());
    }

    void Read(std::string& rData)
    {
        std::size_t received_size = ReceiveSize();
        rData.resize(received_size);
        std::cout << "before READING" << std::endl;
        read(mPipeHandleRead, &(rData.front()), received_size); // using front as other methods that access the underlying char are const
    }

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

    void Close()
    {
        close(mPipeHandleWrite);
        close(mPipeHandleRead);
    }


private:
    int mPipeHandleWrite;
    int mPipeHandleRead;

    fs::path mPipeNameWrite;
    fs::path mPipeNameRead;

    void SendSize(const std::uint64_t Size)
    {
        std::cout << "preparing to send size: " << Size << " (sizeof: " << sizeof(Size) << ")" << std::endl;

        write(mPipeHandleWrite, &Size, sizeof(Size));
    }

    std::uint64_t ReceiveSize()
    {
        std::uint64_t imp_size_u;
        std::cout << "preparing to receive size (sizeof: " << sizeof(imp_size_u) << ")" << std::endl;

        read(mPipeHandleRead, &imp_size_u, sizeof(imp_size_u));

        return imp_size_u;
    }
};



class CO_SIM_IO_API PipeCommunication : public Communication
{
public:
    PipeCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~PipeCommunication() override
    {
        if (GetIsConnected()) {
            CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
            Info tmp;
            Disconnect(tmp);
        }
    }

private:

    using PipeDescriptorType = int; // https://man7.org/linux/man-pages/man2/open.2.html
    using PipeNameType = fs::path;
    using PipeType = std::pair<PipeNameType, PipeDescriptorType>;
    using BidirecionalPipeType = std::pair<PipeType, PipeType>;
    using PipeMapType = std::unordered_map<std::size_t, BidirecionalPipeType>;

    PipeMapType mPipeMap;

    std::shared_ptr<BidirectionalPipeUnix> mpPipe;

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
