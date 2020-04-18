// Kratos   ______     _____ _           ________
//         / ____/___ / ___/(_)___ ___  /  _/ __ |
//        / /   / __ \\__ \/ / __ `__ \ / // / / /
//       / /___/ /_/ /__/ / / / / / / // // /_/ /
//       \____/\____/____/_/_/ /_/ /_/___/\____/
//      Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef KRATOS_CO_SIM_SOCKETS_COMM_H_INCLUDED
#define KRATOS_CO_SIM_SOCKETS_COMM_H_INCLUDED

// System includes

// External includes
#include <boost/asio.hpp>

// Project includes
#include "co_sim_communication.h"

namespace CoSimIO {
namespace Internals {

class CoSimSocketsCommunication : public CoSimCommunication
{
public:
    explicit CoSimSocketsCommunication(const std::string& rName, SettingsType& rSettings, const bool IsConnectionMaster)
        : CoSimCommunication(rName, rSettings, IsConnectionMaster)
    {
       KRATOS_CO_SIM_ERROR << "Sockets Communication is not implemented yet" << std::endl;
    }

};

} // namespace Internals
} // namespace CoSimIO

#endif /* KRATOS_CO_SIM_SOCKETS_COMM_H_INCLUDED */