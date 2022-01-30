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
#include "mpi/includes/communication/mpi_factory.hpp"

#include "mpi/includes/communication/mpi_inter_communication.hpp"

namespace CoSimIO {
namespace Internals {

CommunicationFactory::CommCreateFctsType MPICommunicationFactory::GetCommunicationCreateFunctions() const
{
    auto fcts = CommunicationFactory::GetCommunicationCreateFunctions();

    fcts["mpi_inter"] = [](
        const Info& I_Settings,
        const std::shared_ptr<DataCommunicator> pDataComm){
            return CoSimIO::make_unique<MPIInterCommunication>(I_Settings, pDataComm);};

    return fcts;
}

} // namespace Internals
} // namespace CoSimIO
