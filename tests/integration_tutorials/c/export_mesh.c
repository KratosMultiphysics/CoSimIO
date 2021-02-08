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
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_export_mesh");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_import_mesh");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("fluid_mesh");

    int number_of_nodes=6;
    double nodal_coordinates[] = {
        0.0, 2.5, 1.0, /*1*/
        2.0, 0.0, 1.5, /*2*/
        2.0, 2.5, 1.5, /*3*/
        4.0, 2.5, 1.7, /*4*/
        4.0, 0.0, 1.7, /*5*/
        6.0, 0.0, 1.8  /*6*/
    };

    for (int i=0; i<number_of_nodes; ++i) {
        CoSimIO_ModelPart_CreateNewNode(
            model_part,
            i+1, // Id
            nodal_coordinates[i*3],   // X
            nodal_coordinates[i*3+1], // Y
            nodal_coordinates[i*3+2]  // Z
        );
    }

    int number_of_elements = 4;
    int elements_connectivities[] = {
        1, 2, 3, /*1*/
        2, 4, 3, /*2*/
        2, 5, 4, /*3*/
        4, 5, 6, /*4*/
    };

    const int num_nodes_per_element = 3;
    int connectivity[3];
    for (int i=0; i<number_of_elements; ++i) {
        for (int j=0; j<num_nodes_per_element; ++j) {
            connectivity[j] = elements_connectivities[i*3+j];
        }
        CoSimIO_ModelPart_CreateNewElement(
            model_part,
            i+1, // Id
            CoSimIO_Triangle3D3,
            connectivity
        );
    }

    // Creatint the export_settings
    CoSimIO_Info export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(export_settings, "connection_name", connection_name);

    // Exporting the data
    CoSimIO_Info export_info = CoSimIO_ExportMesh(export_settings, model_part);

    // Freeing the export_info and export_settings
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_settings);

    // Disconnecting at the end
    CoSimIO_Info disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info
    CoSimIO_FreeInfo(disconnect_info);


    return 0;
}
