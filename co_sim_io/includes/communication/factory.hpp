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

#ifndef CO_SIM_IO_COMMUNICATION_FACTORY_INCLUDED
#define CO_SIM_IO_COMMUNICATION_FACTORY_INCLUDED

// System includes

// Project includes
#include "communication.hpp"

namespace CoSimIO {
namespace Internals {

std::unique_ptr<Communication> CO_SIM_IO_API CreateCommunication(
    const Info& I_Settings,
    const std::shared_ptr<DataCommunicator> pDataComm);

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_COMMUNICATION_FACTORY_INCLUDED
