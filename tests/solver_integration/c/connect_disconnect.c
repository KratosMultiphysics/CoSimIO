//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Pooyan Dadvand
//

// CoSimulation includes
#include "co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    CoSimIO_Info settings;
    CoSimIO_Info_SetString(settings, "connection_name", "test_connection"); // The connection name must be unique for each connection between two solvers
    CoSimIO_Info_SetString(settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(settings, "echo_level", 1);
    CoSimIO_Info_SetString(settings, "solver_version", "1.25");

    CoSimIO_Info info = CoSimIO_Connect(settings);

    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(info, "connection_status"), CoSimIO_Connected);

    info = CoSimIO_Disconnect(settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(info, "connection_status"), CoSimIO_Disconnected);

    return 0;
}