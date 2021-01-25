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
#include <string.h>

// CoSimulation includes
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL_INT(a, b)                            \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

#define COSIMIO_CHECK_EQUAL_DOUBLE(a, b)                         \
    if (a != b) {                                                \
        printf("in line %d : %f is not equalt to %f\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    // Creating the connection settings
    CoSimIO_Info connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_import_mesh");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_export_mesh");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    char connection_name[BUFSIZ];
    strcpy(connection_name, CoSimIO_Info_GetString(connect_info, "connection_name"));
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info

    // After conneting we may import the mesh
    double* nodal_coordinates;
    int number_of_nodes = 0;
    int* elements_connectivities;
    int number_of_elements_connectivities = 0;
    int* elements_types;
    int number_of_elements = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(import_settings, "connection_name", connection_name);

    // Importing the mesh
    CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings
        , &number_of_nodes,&number_of_elements,&number_of_elements_connectivities
        , &nodal_coordinates, &elements_connectivities, &elements_types);

    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    // Checking the imported mesh
    int expected_number_of_nodes=6;
    double expected_nodal_coordinates[] = {
        0.0, 2.5, 1.0, /*0*/
        2.0, 0.0, 1.5, /*1*/
        2.0, 2.5, 1.5, /*2*/
        4.0, 2.5, 1.7, /*3*/
        4.0, 0.0, 1.7, /*4*/
        6.0, 0.0, 1.8  /*5*/
    };

    int expected_number_of_elements_connectivities = 12;
    int expected_elements_connectivities[] = {
        0, 1, 2, /*1*/
        1, 3, 2, /*2*/
        1, 4, 3, /*3*/
        3, 4, 5, /*4*/
    };

    int expected_number_of_elements = 4;
    int expected_elements_types[] = {5,5,5,5}; // VTK_TRIANGLE

    COSIMIO_CHECK_EQUAL_INT(expected_number_of_nodes,  number_of_nodes);
    COSIMIO_CHECK_EQUAL_INT(expected_number_of_elements_connectivities,  number_of_elements_connectivities);
    COSIMIO_CHECK_EQUAL_INT(expected_number_of_elements,  number_of_elements);

    for(int i = 0 ; i <  number_of_nodes * 3 ; i++)
        COSIMIO_CHECK_EQUAL_DOUBLE(expected_nodal_coordinates[i],  nodal_coordinates[i]);

    for(int i = 0 ; i <  number_of_elements_connectivities ; i++)
        COSIMIO_CHECK_EQUAL_INT(expected_elements_connectivities[i],  elements_connectivities[i]);

    for(int i = 0 ; i <  number_of_elements ; i++)
        COSIMIO_CHECK_EQUAL_INT(expected_elements_types[i],  elements_types[i]);


    // Disconnecting at the end
    CoSimIO_Info disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
