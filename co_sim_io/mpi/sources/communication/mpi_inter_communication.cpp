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

// External includes

// Project includes
#include "mpi/includes/communication/mpi_inter_communication.hpp"

namespace CoSimIO {
namespace Internals {


MPIInterCommunication::MPIInterCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
}

MPIInterCommunication::~MPIInterCommunication()
{
    if (GetIsConnected()) {
        CO_SIM_IO_INFO("CoSimIO") << "Warning: Disconnect was not performed, attempting automatic disconnection!" << std::endl;
        Info tmp;
        Disconnect(tmp);
    }
}

Info MPIInterCommunication::ConnectDetail(const Info& I_Info)
{
    // mpPipe = std::make_shared<BidirectionalPipe>(
    //     GetCommunicationDirectory(),
    //     GetConnectionName() + "_r" + std::to_string(GetDataCommunicator().Rank()),
    //     GetIsPrimaryConnection(),
    //     GetPipeBufferSize(I_Info),
    //     GetEchoLevel());

    return Info(); // TODO use
}

Info MPIInterCommunication::DisconnectDetail(const Info& I_Info)
{
    // mpPipe->Close();
    return Info(); // TODO use
}

void MPIInterCommunication::DerivedHandShake() const
{
    // CO_SIM_IO_ERROR_IF(GetMyInfo().Get<std::string>("operating_system") != GetPartnerInfo().Get<std::string>("operating_system")) << "Pipe communication cannot be used between different operating systems!" << std::endl;

    // const std::size_t my_use_buffer_size = GetMyInfo().Get<Info>("communication_settings").Get<std::size_t>("buffer_size");
    // const std::size_t partner_buffer_size = GetPartnerInfo().Get<Info>("communication_settings").Get<std::size_t>("buffer_size");
    // CO_SIM_IO_ERROR_IF(my_use_buffer_size != partner_buffer_size) << "Mismatch in buffer_size!\nMy buffer_size: " << my_use_buffer_size << "\nPartner buffer_size: " << partner_buffer_size << std::noboolalpha << std::endl;
}

Info MPIInterCommunication::GetCommunicationSettings() const
{
    CO_SIM_IO_TRY

    Info info;
    // info.Set("buffer_size", mBufferSize);
    return info;

    CO_SIM_IO_CATCH
}

double MPIInterCommunication::SendString(
    const Info& I_Info,
    const std::string& rData)
{
    // return mpPipe->Write(rData, 1);
    return 0.0;
}

double MPIInterCommunication::ReceiveString(
    const Info& I_Info,
    std::string& rData)
{
    // return mpPipe->Read(rData, 1);
    return 0.0;
}

double MPIInterCommunication::SendDataContainer(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    // return mpPipe->Write(rData, sizeof(double));
    return 0.0;
}

double MPIInterCommunication::ReceiveDataContainer(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    // return mpPipe->Read(rData, sizeof(double));
    return 0.0;
}

} // namespace Internals
} // namespace CoSimIO
