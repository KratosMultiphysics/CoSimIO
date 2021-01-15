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
    // Creating the connection settings
    CoSimIO_Info connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "connection_name", "mesh_mapping"); // This should be the same in both side
    CoSimIO_Info_SetString(connection_settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "solver_version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info

    // Creating the export origin mesh settings
    CoSimIO_Info export_mesh_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_origin");
    CoSimIO_Info_SetString(export_mesh_settings, "connection_name", "mesh_mapping");

    // defining the origin mesh
    int number_of_nodes_o=6;
    double nodal_coordinates_o[] = {
        0.0, 2.5, 0.0, /*0*/
        2.0, 0.0, 0.0, /*1*/
        2.0, 2.5, 0.0, /*2*/
        4.0, 2.5, 0.0, /*3*/
        4.0, 0.0, 0.0, /*4*/
        6.0, 0.0, 0.0  /*5*/
    };

    int number_of_elements_connectivities_o = 12;
    int elements_connectivities_o[] = {
        0, 1, 2, /*1*/
        1, 3, 2, /*2*/
        1, 4, 3, /*3*/
        3, 4, 5 /*4*/
    };

    int number_of_elements_o = 4;
    int elements_types_o[] = {5,5,5,5}; // VTK_TRIANGLE


    // Exporting the origin mesh
    CoSimIO_Info export_info_o = CoSimIO_ExportMesh(export_mesh_settings
        , number_of_nodes_o, number_of_elements_o,        number_of_elements_connectivities_o
        , nodal_coordinates_o, elements_connectivities_o, elements_types_o);


    // Creating the export destination mesh settings

    // defining the destination mesh
    int number_of_nodes_d=6;
    double nodal_coordinates_d[] = {
        0.2, 2.7, 0.0, /*0*/
        2.2, 0.2, 0.0, /*1*/
        2.2, 2.7, 0.0, /*2*/
        4.2, 2.7, 0.0, /*3*/
        4.2, 0.2, 0.0, /*4*/
        6.2, 0.2, 0.0  /*5*/
    };

    int number_of_elements_connectivities_d = 12;
    int elements_connectivities_d[] = {
        0, 1, 2, /*1*/
        1, 3, 2, /*2*/
        1, 4, 3, /*3*/
        3, 4, 5 /*4*/
    };

    int number_of_elements_d = 4;
    int elements_types_d[] = {5,5,5,5}; // VTK_TRIANGLE

    // Exporting the destination mesh
    CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_destination");
    CoSimIO_Info export_info_d = CoSimIO_ExportMesh(export_mesh_settings
        , number_of_nodes_d, number_of_elements_d,      number_of_elements_connectivities_d
        , nodal_coordinates_d, elements_connectivities_d, elements_types_d);

    // Free memory
    CoSimIO_FreeInfo(export_info_o);
    CoSimIO_FreeInfo(export_info_d);
    CoSimIO_FreeInfo(export_mesh_settings);

    // Creating the export origin data settings
    CoSimIO_Info export_data_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_data_settings, "identifier", "data_to_map");
    CoSimIO_Info_SetString(export_data_settings, "connection_name", "mesh_mapping");

    // Exporting the origin data
    double export_data[] = {1, 1.5, 2, 2.5, 3, 3.5};
    CoSimIO_Info export_info = CoSimIO_ExportData(export_data_settings, number_of_nodes_o, export_data);

    // Free memory
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_data_settings);


    // Creating the import data settings
    CoSimIO_Info import_mapped_data_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_mapped_data_settings, "identifier", "mapped_data");
    CoSimIO_Info_SetString(import_mapped_data_settings, "connection_name", "mesh_mapping");

    // Importing the data
    double* mapped_data;
    int data_allocated_size = 0;
    CoSimIO_Info import_info = CoSimIO_ImportData(import_mapped_data_settings, &data_allocated_size, &mapped_data);

    // Free memory
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_mapped_data_settings);

    // Disconnecting at the end
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(connection_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
