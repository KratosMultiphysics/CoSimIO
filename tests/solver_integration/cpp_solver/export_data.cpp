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
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

int main()
{
    CoSimIO::ConnectionSettings settings;
    settings.Set("connection_name", "test_connection"); // This must be unique for each connection between two solvers
    settings.Set("solver_name", "solver_1"); // Not to be confused with the connection name.
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto return_info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(return_info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);

    std::vector<double> data_to_send(4,3.14);
    CoSimIO::Info info;
    info.Set("identifier", "vector_of_pi");
    info.Set("connection_name", "test_connection");
    return_info = CoSimIO::ExportData(info, data_to_send);

    return_info = CoSimIO::Disconnect(settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(return_info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}