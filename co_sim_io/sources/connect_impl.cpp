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
#include <string>

// Project includes
#include "includes/connect_impl.hpp"
#include "includes/utilities.hpp"

namespace CoSimIO {
namespace Internals {

Info ConnectImpl(const Info& I_Settings, std::shared_ptr<DataCommunicator> I_DataComm)
{
    const std::string my_name = I_Settings.Get<std::string>("my_name");
    const std::string connect_to = I_Settings.Get<std::string>("connect_to");

    // perform some checks
    Internals::CheckEntry(my_name, "my_name");
    Internals::CheckEntry(connect_to, "connect_to");
    CO_SIM_IO_ERROR_IF(my_name == connect_to) << "Connecting to self is not allowed!" << std::endl;

    const std::string connection_name = Internals::CreateConnectionName(my_name, connect_to);

    CO_SIM_IO_ERROR_IF(HasIO(connection_name)) << "A connection from \"" << my_name << "\" to \"" << connect_to << "\"already exists!" << std::endl;

    s_co_sim_connections[connection_name] = std::unique_ptr<Internals::Connection>(new Internals::Connection(I_Settings));

    auto info = GetConnection(connection_name).Connect(I_Settings);
    info.Set<std::string>("connection_name", connection_name);

    return info;
}

} // namespace Internals
} // namespace CoSimIO
