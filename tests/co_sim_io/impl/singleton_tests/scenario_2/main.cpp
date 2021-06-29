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

/*
Testing if the CoSimIO can be safely included in complex libraries
In this scenario the CoSimIO is included in the main executable
and in the external library
Connecting and disconnecting is done in the external library
*/

// CoSimulation includes
#include "co_sim_io.hpp"
#include "ext_lib.hpp"

#define COSIMIO_CHECK_True(a)                                    \
    if (!a) {                                                    \
        std::cout << "in line " << __LINE__ << " : "             \
                  << "false is not true" << std::endl;           \
        return 1;                                                \
    }

int main()
{
    const std::string connection_name = ConnectToCoSimIO();

    COSIMIO_CHECK_True(CoSimIO::Internals::HasIO(connection_name));

    DisconnectFromCoSimIO(connection_name);

    return 0;
}
