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
    CoSimIO_Info_SetString(connection_settings, "connection_name", "im_exp_mesh"); // This should be the same in both side
    CoSimIO_Info_SetString(connection_settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "solver_version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info

    int export_number_of_nodes=6;
    double export_nodal_coordinates[] = {
        0.0, 2.5, 1.0, /*0*/
        2.0, 0.0, 1.5, /*1*/
        2.0, 2.5, 1.5, /*2*/
        4.0, 2.5, 1.7, /*3*/
        4.0, 0.0, 1.7, /*4*/
        6.0, 0.0, 1.8  /*5*/
    };

    int export_number_of_elements_connectivities = 12;
    int export_elements_connectivities[] = {
        0, 1, 2, /*1*/
        1, 3, 2, /*2*/
        1, 4, 3, /*3*/
        3, 4, 5, /*4*/
    };

    int export_number_of_elements = 4;
    int export_elements_types[] = {5,5,5,5}; // VTK_TRIANGLE

    // Creatint the export_settings
    CoSimIO_Info export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "mesh_exchange_1");
    CoSimIO_Info_SetString(export_settings, "connection_name", "im_exp_mesh");

    // Exporting the data
    CoSimIO_Info export_info = CoSimIO_ExportMesh(export_settings
        , export_number_of_nodes,export_number_of_elements,export_number_of_elements_connectivities
        , export_nodal_coordinates, export_elements_connectivities, export_elements_types);

    // Freeing the export_info and export_settings
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_settings);

    // Now we import back the mesh
    double* import_nodal_coordinates;
    int import_number_of_nodes = 0;
    int* import_elements_connectivities;
    int import_number_of_elements_connectivities = 0;
    int* import_elements_types;
    int import_number_of_elements = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "mesh_exchange_2");
    CoSimIO_Info_SetString(import_settings, "connection_name", "im_exp_mesh");

    // Importing the mesh
    CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings
        , &import_number_of_nodes,&import_number_of_elements,&import_number_of_elements_connectivities
        , &import_nodal_coordinates, &import_elements_connectivities, &import_elements_types);

    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    COSIMIO_CHECK_EQUAL_INT(export_number_of_nodes, import_number_of_nodes);
    COSIMIO_CHECK_EQUAL_INT(export_number_of_elements_connectivities, import_number_of_elements_connectivities);
    COSIMIO_CHECK_EQUAL_INT(export_number_of_elements, import_number_of_elements);

    for(int i = 0 ; i < import_number_of_nodes * 3 ; i++)
        COSIMIO_CHECK_EQUAL_DOUBLE(export_nodal_coordinates[i], import_nodal_coordinates[i]);

    for(int i = 0 ; i < import_number_of_elements_connectivities ; i++)
        COSIMIO_CHECK_EQUAL_INT(export_elements_connectivities[i], import_elements_connectivities[i]);

    for(int i = 0 ; i < import_number_of_elements ; i++)
        COSIMIO_CHECK_EQUAL_INT(export_elements_types[i], import_elements_types[i]);

    // Disconnecting at the end
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(connection_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
