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
Connecting and disconnecting is done in the external library 1
The it is checked if the connection is also available in library 2
Difference to scenario 4 is that the main doesn't include CoSimIO
*/

// System includes
#include <iostream>

// CoSimulation includes
#include "ext_lib_1.hpp"
#include "ext_lib_2.hpp"

#define COSIMIO_CHECK_True(a)                                    \
    if (!a) {                                                    \
        std::cout << "in line " << __LINE__ << " : "             \
                  << "false is not true" << std::endl;           \
        return 1;                                                \
    }

int main()
{
    const std::string connection_name = ConnectToCoSimIO();

    COSIMIO_CHECK_True(ExtLib2HasIO(connection_name));

    DisconnectFromCoSimIO(connection_name);

    return 0;
}