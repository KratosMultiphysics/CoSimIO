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

namespace CoSimIO {
namespace Internals {

std::unique_ptr<Communication> MPICommunicationFactory::Create(
    const Info& I_Settings,
    const std::shared_ptr<DataCommunicator> pDataComm) const
{
    return CommunicationFactory::Create(I_Settings, pDataComm);
}

} // namespace Internals
} // namespace CoSimIO
