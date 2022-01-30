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

#ifndef CO_SIM_IO_MPI_INTER_COMMUNICATION_INCLUDED
#define CO_SIM_IO_MPI_INTER_COMMUNICATION_INCLUDED

// System includes

// Project includes
#include "includes/communication/communication.hpp"

namespace CoSimIO {
namespace Internals {

class CO_SIM_IO_API MPIInterCommunication : public Communication
{
public:
    MPIInterCommunication(
        const Info& I_Settings,
        std::shared_ptr<DataCommunicator> I_DataComm);

    ~MPIInterCommunication() override;

    std::string GetCommunicationName() const override {return "mpi_inter";}

    Info ConnectDetail(const Info& I_Info) override;

    Info DisconnectDetail(const Info& I_Info) override;

    double SendString(
        const Info& I_Info,
        const std::string& rData) override;

    double ReceiveString(
        const Info& I_Info,
        std::string& rData) override;

    double SendDataContainer(
        const Info& I_Info,
        const Internals::DataContainer<double>& rData) override;

    double ReceiveDataContainer(
        const Info& I_Info,
        Internals::DataContainer<double>& rData) override;

    void DerivedHandShake() const override;

    Info GetCommunicationSettings() const override;
};

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_MPI_INTER_COMMUNICATION_INCLUDED