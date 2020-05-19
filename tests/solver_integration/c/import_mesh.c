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

// CoSimulation includes
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    // Creating the connection settings
    CoSimIO_Info connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "connection_name", "test_connection"); // The connection name must be unique for each connection between two solvers
    CoSimIO_Info_SetString(connection_settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "solver_version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info

    // After conneting we may import the data
    double* nodal_coordinates;
    int number_of_nodes = 0;
    int* elements_connectivities;
    int number_of_elements_connectivities = 0;
    int* elements_types;
    int number_of_elements = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(import_settings, "connection_name", "test_connection");

    // Importing the mesh
    CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings
        , &number_of_nodes,&number_of_elements,&number_of_elements_connectivities
        , &nodal_coordinates, &elements_connectivities, &elements_types);

    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    // Disconnecting at the end
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(connection_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}