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
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    // CoSimIO::Info settings;
    // settings.Set("connection_name", "test_connection"); // This must be unique for each connection between two solvers
    // settings.Set("solver_name", "solver_1"); // Not to be confused with the connection name.
    // settings.Set("echo_level", 1);
    // settings.Set("solver_version", "1.25");

    // auto info = CoSimIO::Connect(settings);
    // COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);

    // std::vector<double> receive_data;
    // info.Clear();
    // info.Set("identifier", "vector_of_pi");
    // info.Set("connection_name", "test_connection");
    // info = CoSimIO::ImportData(info, receive_data);

    // for(auto& value : receive_data)
    //     COSIMIO_CHECK_EQUAL(value, 3.14);

    // info = CoSimIO::Disconnect(settings); // disconnect afterwards
    // COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}