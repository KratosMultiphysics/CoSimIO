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
    CoSimIO::Info settings;
    settings.Set("connection_name", "test_connection"); // This must be unique for each connection between two solvers
    settings.Set("solver_name", "solver_1"); // Not to be confused with the connection name.
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);

    std::vector<double> nodal_coordinates{
        0.0, 2.5, 1.0, /*0*/
        2.0, 0.0, 1.5, /*1*/
        2.0, 2.5, 1.5, /*2*/
        4.0, 2.5, 1.7, /*3*/
        4.0, 0.0, 1.7, /*4*/
        6.0, 0.0, 1.8  /*5*/
    };

    std::vector<int> elements_connectivities = {
        0, 1, 2, /*1*/
        1, 3, 2, /*2*/
        1, 4, 3, /*3*/
        3, 4, 5, /*4*/
    };

    std::vector<int> elements_types = {5,5,5,5}; // VTK_TRIANGLE
    info.Clear();
    info.Set("identifier", "vector_of_pi");
    info.Set("connection_name", "test_connection");

    info = CoSimIO::ExportMesh(info,nodal_coordinates, elements_connectivities, elements_types);

    info = CoSimIO::Disconnect(settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}