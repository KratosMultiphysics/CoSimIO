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
#include "includes/define.hpp" // for "CO_SIM_IO_COMPILED_IN_WINDOWS"

#ifdef CO_SIM_IO_COMPILED_IN_WINDOWS

#else
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

// External includes

// Project includes
#include "includes/communication/pipe_communication.hpp"
#include "includes/stream_serializer.hpp"
#include "includes/utilities.hpp"

namespace CoSimIO {
namespace Internals {

PipeCommunication::PipeCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

PipeCommunication::~PipeCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info PipeCommunication::ConnectDetail(const Info& I_Info)
{
    mpPipe = std::make_shared<BidirectionalPipe>(
        GetCommunicationDirectory(),
        GetConnectionName() + "_r" + std::to_string(GetDataCommunicator().Rank()),
        GetIsPrimaryConnection(),
        GetPipeBufferSize(I_Info));

    return Info(); // TODO use
}

Info PipeCommunication::DisconnectDetail(const Info& I_Info)
{
    mpPipe->Close();
    return Info(); // TODO use
}

void PipeCommunication::DerivedHandShake() const
{
    CO_SIM_IO_ERROR_IF(GetMyInfo().Get<std::string>("operating_system") != GetPartnerInfo().Get<std::string>("operating_system")) << "Pipe communication cannot be used between different operating systems!" << std::endl;
}


PipeCommunication::BidirectionalPipe::BidirectionalPipe(
    const fs::path& rPipeDir,
    const fs::path& rBasePipeName,
    const bool IsPrimary,
    const std::size_t BufferSize) : mBufferSize(BufferSize)
{
    mPipeNameWrite = mPipeNameRead = rPipeDir / rBasePipeName;

    #ifdef CO_SIM_IO_COMPILED_IN_WINDOWS
    CO_SIM_IO_ERROR << "Pipe communication is not yet implemented for Windows!" << std::endl;
    #else
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

        Utilities::WaitUntilPathExists(mPipeNameWrite); // created last hence wait for it

        CO_SIM_IO_ERROR_IF((mPipeHandleRead = open(mPipeNameRead.c_str(), O_RDONLY)) < 0) << "Pipe " << mPipeNameRead << " could not be opened!" << std::endl;
        CO_SIM_IO_ERROR_IF((mPipeHandleWrite = open(mPipeNameWrite.c_str(), O_WRONLY)) < 0) << "Pipe " << mPipeNameWrite << " could not be opened!" << std::endl;
    }
    #endif
}

void PipeCommunication::BidirectionalPipe::Close()
{
    #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
    close(mPipeHandleWrite);
    close(mPipeHandleRead);
    #endif
}

void PipeCommunication::BidirectionalPipe::SendSize(const std::uint64_t Size)
{
    #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
    const ssize_t bytes_written = write(mPipeHandleWrite, &Size, sizeof(Size));
    CO_SIM_IO_ERROR_IF(bytes_written < 0) << "Error in writing to Pipe!" << std::endl;
    #endif
}

std::uint64_t PipeCommunication::BidirectionalPipe::ReceiveSize()
{
    #ifndef CO_SIM_IO_COMPILED_IN_WINDOWS
    std::uint64_t imp_size_u;
    const ssize_t bytes_read = read(mPipeHandleRead, &imp_size_u, sizeof(imp_size_u));
    CO_SIM_IO_ERROR_IF(bytes_read < 0) << "Error in reading from Pipe!" << std::endl;
    return imp_size_u;
    #else
    return 0;
    #endif
}

double PipeCommunication::SendString(const std::string& rData)
{
    return mpPipe->Write(rData, 1);
}

double PipeCommunication::ReceiveString(std::string& rData)
{
    return mpPipe->Read(rData, 1);
}

double PipeCommunication::SendDataContainer(const Internals::DataContainer<double>& rData)
{
    return mpPipe->Write(rData, sizeof(double));
}

double PipeCommunication::ReceiveDataContainer(Internals::DataContainer<double>& rData)
{
    return mpPipe->Read(rData, sizeof(double));
}

std::size_t PipeCommunication::GetPipeBufferSize(const Info& I_Info) const
{
    return I_Info.Get<int>("buffer_size", 8192);
}

} // namespace Internals
} // namespace CoSimIO
