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

// Project includes
#include "mpi/includes/communication/mpi_inter_communication.hpp"
#include "mpi/includes/mpi_data_communicator.hpp"

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

struct PortsConnectionInfo
{
    std::string PortName;

    friend class CoSimIO::Internals::Serializer;

    void save(CoSimIO::Internals::Serializer& rSerializer) const
    {
        rSerializer.save("PortName", PortName);
    }

    void load(CoSimIO::Internals::Serializer& rSerializer)
    {
        rSerializer.load("PortName", PortName);
    }
};

// this is required as the serializer cannot handle newlines
void PrepareStringForAsciiSerialization(std::string& rString)
{
    const char disallowed_chars[] = {'<', '>'};
    for (const auto ch : disallowed_chars) {
        CO_SIM_IO_ERROR_IF_NOT(rString.find(ch) == std::string::npos) << "String contains a character that is not allowed: \"" << std::string(1,ch) << "\"!" << std::endl;
    }

    std::replace(rString.begin(), rString.end(), '\n', '<');
    std::replace(rString.begin(), rString.end(), '"', '>');
}

void RevertAsciiSerialization(std::string& rString)
{
    std::replace(rString.begin(), rString.end(), '<', '\n');
    std::replace(rString.begin(), rString.end(), '>', '"');
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

    if (!GetIsPrimaryConnection()) {GetConnectionInformation();}

    MPI_Comm my_comm = MPIDataCommunicator::GetMPICommunicator(GetDataCommunicator());

    if (GetIsPrimaryConnection()) {
        MPI_Comm_accept(mPortName.c_str(), MPI_INFO_NULL, 0, my_comm, &mInterComm); // todo check return code
    } else {
        MPI_Comm_connect(mPortName.c_str(), MPI_INFO_NULL, 0, my_comm, &mInterComm); // todo check return code
    }


    // SYNC ???


    return Info(); // TODO use

    CO_SIM_IO_CATCH
}

Info MPIInterCommunication::DisconnectDetail(const Info& I_Info)
{
    CO_SIM_IO_TRY

    MPI_Comm_disconnect(&mInterComm); // todo check return code

    // SYNC => probably not necessary as "MPI_Comm_disconnect" is collective

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

        const auto& r_data_comm = GetDataCommunicator();
        std::vector<PortsConnectionInfo> conn_infos;

        PortsConnectionInfo my_conn_info {mPortName};

        if (r_data_comm.Rank() == 0) {
            conn_infos.resize(r_data_comm.Size());
            conn_infos[0] = {mPortName};
            for (int i=1; i<r_data_comm.Size(); ++i) {
                r_data_comm.Recv(conn_infos[i], i);
            }
        } else {
            r_data_comm.Send(my_conn_info, 0);
        }

        StreamSerializer serializer(GetSerializerTraceType());
        serializer.save("conn_info", conn_infos);
        mSerializedConnectionInfo = serializer.GetStringRepresentation();
        if (GetSerializerTraceType() != Serializer::TraceType::SERIALIZER_NO_TRACE) {
            PrepareStringForAsciiSerialization(mSerializedConnectionInfo);
        }
    }

    CO_SIM_IO_CATCH
}

void MPIInterCommunication::GetConnectionInformation()
{
    CO_SIM_IO_TRY

    CO_SIM_IO_ERROR_IF(GetIsPrimaryConnection()) << "This function can only be used as secondary connection!" << std::endl;

    const auto partner_info = GetPartnerInfo();
    std::string serialized_info = partner_info.Get<Info>("communication_settings").Get<std::string>("connection_info");

    if (GetSerializerTraceType() != Serializer::TraceType::SERIALIZER_NO_TRACE) {
        RevertAsciiSerialization(serialized_info);
    }

    std::vector<PortsConnectionInfo> conn_infos;

    StreamSerializer serializer(serialized_info, GetSerializerTraceType());
    serializer.load("conn_info", conn_infos);

    CO_SIM_IO_ERROR_IF(static_cast<int>(conn_infos.size()) != GetDataCommunicator().Size()) << "Wrong number of connection infos!" << std::endl;

    const auto& my_conn_info = conn_infos[GetDataCommunicator().Rank()];
    mPortName = my_conn_info.PortName;

    CO_SIM_IO_CATCH
}

Info MPIInterCommunication::GetCommunicationSettings() const
{
    CO_SIM_IO_TRY

    Info info;

    if (GetIsPrimaryConnection() && GetDataCommunicator().Rank() == 0) {
        info.Set("connection_info", mSerializedConnectionInfo);
    }

    return info;

    CO_SIM_IO_CATCH
}

double MPIInterCommunication::SendString(
    const Info& I_Info,
    const std::string& rData)
{
    CO_SIM_IO_TRY

    const auto start_time(std::chrono::steady_clock::now());

    MPI_Send(
        rData.data(),
        rData.size(),
        MPI_CHAR,
        GetDataCommunicator().Rank(),
        0,
        mInterComm); // todo check return code

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

double MPIInterCommunication::ReceiveString(
    const Info& I_Info,
    std::string& rData)
{
    CO_SIM_IO_TRY

    const int size = ReceiveSize(mInterComm, MPI_CHAR, GetDataCommunicator().Rank()); // serves also as synchronization for time measurement
    rData.resize(size);

    const auto start_time(std::chrono::steady_clock::now());

    MPI_Recv(
        &(rData.front()),
        rData.size(),
        MPI_CHAR,
        GetDataCommunicator().Rank(),
        0,
        mInterComm,
        MPI_STATUS_IGNORE); // todo check return code

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

double MPIInterCommunication::SendDataContainer(
    const Info& I_Info,
    const Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    const auto start_time(std::chrono::steady_clock::now());

    MPI_Send(
        rData.data(),
        rData.size(),
        MPI_DOUBLE,
        GetDataCommunicator().Rank(),
        0,
        mInterComm); // todo check return code

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

double MPIInterCommunication::ReceiveDataContainer(
    const Info& I_Info,
    Internals::DataContainer<double>& rData)
{
    CO_SIM_IO_TRY

    const int size = ReceiveSize(mInterComm, MPI_DOUBLE, GetDataCommunicator().Rank()); // serves also as synchronization for time measurement
    rData.resize(size);

    const auto start_time(std::chrono::steady_clock::now());

    MPI_Recv(
        rData.data(),
        rData.size(),
        MPI_DOUBLE,
        GetDataCommunicator().Rank(),
        0,
        mInterComm,
        MPI_STATUS_IGNORE); // todo check return code

    return Utilities::ElapsedSeconds(start_time);

    CO_SIM_IO_CATCH
}

} // namespace Internals
} // namespace CoSimIO
