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

namespace CoSimIO {
namespace Internals {

PipeCommunication::PipeCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

Info PipeCommunication::ConnectDetail(const Info& I_Info)
{
    mpPipe = std::make_shared<BidirectionalPipeUnix>(GetCommunicationDirectory(), GetConnectionName(), GetIsPrimaryConnection());

    return Info(); // TODO use
}

Info PipeCommunication::DisconnectDetail(const Info& I_Info)
{
    mpPipe->Close();
    return Info(); // TODO use
}

Info PipeCommunication::ImportInfoImpl(const Info& I_Info)
{
    Info imported_info;
    mpPipe->Receive(imported_info);
    return imported_info;
}

Info PipeCommunication::ExportInfoImpl(const Info& I_Info)
{
    mpPipe->Send(I_Info);
    return Info(); // TODO use
}

Info PipeCommunication::ImportDataImpl(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    mpPipe->Receive(rData);
    return Info(); // TODO use
}

Info PipeCommunication::ExportDataImpl(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    mpPipe->Send(rData);
    return Info(); // TODO use
}

Info PipeCommunication::ImportMeshImpl(
    const Info& I_Info,
    ModelPart& O_ModelPart)
{
    mpPipe->Receive(O_ModelPart);
    return Info(); // TODO use
}

Info PipeCommunication::ExportMeshImpl(
    const Info& I_Info,
    const ModelPart& I_ModelPart)
{
    mpPipe->Send(I_ModelPart);
    return Info(); // TODO use
}

} // namespace Internals
} // namespace CoSimIO
