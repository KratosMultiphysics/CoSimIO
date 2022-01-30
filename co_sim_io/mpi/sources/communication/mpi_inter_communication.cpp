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
#include "mpi.h"

// Project includes
#include "mpi/includes/communication/mpi_inter_communication.hpp"

namespace CoSimIO {
namespace Internals {

namespace {

template<typename TMPIDataType>
int ReceiveSize(
    MPI_Comm Comm,
    TMPIDataType DataType,
    const int Rank)
{
    int size;
    MPI_Status status;
    MPI_Probe(Rank, 0, Comm, &status);
    MPI_Get_count(&status, DataType, &size);
    return size;
}

}


MPIInterCommunication::MPIInterCommunication(
    const Info& I_Settings,
    std::shared_ptr<DataCommunicator> I_DataComm)
    : Communication(I_Settings, I_DataComm)
{
    CO_SIM_IO_ERROR_IF_NOT(I_DataComm->IsDistributed()) << "MPI communication only works with a MPIDataCommunicator!" << std::endl;
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
    CO_SIM_IO_TRY

    if (GetIsPrimaryConnection()) {
        // MPI_Comm_accept(mPortName, MPI_INFO_NULL, 0, MPI_Comm comm, MPI_Comm *newcomm) // todo check return code
    } else {
        // MPI_Comm_connect(mPortName, MPI_INFO_NULL, 0, MPI_Comm comm, MPI_Comm *newcomm) // todo check return code
    }


    // SYNC


    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info MPIInterCommunication::DisconnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    // MPI_Comm_disconnect(MPI_Comm *comm) // todo check return code

    // SYNC

    if (GetIsPrimaryConnection()) {
        MPI_Close_port(mPortName.c_str()); // todo check return code
    }

    return Info(); // TODO use
    CO_SIM_IO_CATCH
}
void MPIInterCommunication::PrepareConnection(const Info& I_Info)
{
    CO_SIM_IO_TRY

    if (GetIsPrimaryConnection()) {
        mPortName.resize(MPI_MAX_PORT_NAME);
        MPI_Open_port(MPI_INFO_NULL, &mPortName.front()); // todo check return code

        std::cout << "Rank: " << GetDataCommunicator().Rank() << " PORT NAME: " << mPortName << std::endl;
    }

    // Rank: 0 PORT NAME: 3326935041.0:1705561554
    // Rank: 1 PORT NAME: 3326935041.1:1705561554
    // Rank: 2 PORT NAME: 3326935041.2:1705561554



    CO_SIM_IO_CATCH
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
    MPI_Send(
        rData.data(),
        rData.size(),
        MPI_CHAR,
        GetDataCommunicator().Rank(),
        0,
        MPI_COMM_WORLD); // todo check return code

    return 0.0;
}

double MPIInterCommunication::ReceiveString(
    const Info& I_Info,
    std::string& rData)
{
    const int size = ReceiveSize(MPI_COMM_WORLD, MPI_CHAR, GetDataCommunicator().Rank());
    rData.resize(size);

    MPI_Recv(
        &(rData.front()),
        rData.size(),
        MPI_CHAR,
        GetDataCommunicator().Rank(),
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // todo check return code

    return 0.0;
}

double MPIInterCommunication::SendDataContainer(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    MPI_Send(
        rData.data(),
        rData.size(),
        MPI_DOUBLE,
        GetDataCommunicator().Rank(),
        0,
        MPI_COMM_WORLD); // todo check return code

    return 0.0;
}

double MPIInterCommunication::ReceiveDataContainer(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    const int size = ReceiveSize(MPI_COMM_WORLD, MPI_DOUBLE, GetDataCommunicator().Rank());
    rData.resize(size);

    MPI_Recv(
        rData.data(),
        rData.size(),
        MPI_DOUBLE,
        GetDataCommunicator().Rank(),
        0,
        MPI_COMM_WORLD,
        MPI_STATUS_IGNORE); // todo check return code

    return 0.0;
}

} // namespace Internals
} // namespace CoSimIO
