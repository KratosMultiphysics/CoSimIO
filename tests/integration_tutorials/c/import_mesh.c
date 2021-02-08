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
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    // After conneting we may import the mesh
    CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("fluid_mesh");

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(import_settings, "connection_name", connection_name);

    // Importing the mesh
    CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings, model_part);

    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    // Checking the imported mesh
    int expected_number_of_nodes = 6;
    double expected_nodal_coordinates[] = {
        0.0, 2.5, 1.0, /*1*/
        2.0, 0.0, 1.5, /*2*/
        2.0, 2.5, 1.5, /*3*/
        4.0, 2.5, 1.7, /*4*/
        4.0, 0.0, 1.7, /*5*/
        6.0, 0.0, 1.8  /*6*/
    };

    int expected_elements_connectivities[] = {
        1, 2, 3, /*1*/
        2, 4, 3, /*2*/
        2, 5, 4, /*3*/
        4, 5, 6, /*4*/
    };

    int expected_number_of_elements = 4;

    COSIMIO_CHECK_EQUAL_INT(CoSimIO_ModelPart_NumberOfNodes(model_part), expected_number_of_nodes);
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_ModelPart_NumberOfElements(model_part), expected_number_of_elements);

    for (int i=0; i<expected_number_of_nodes; ++i) {
        CoSimIO_Node node = CoSimIO_ModelPart_GetNodeByIndex(model_part, i);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Node_Id(node), i+1);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_X(node), expected_nodal_coordinates[i*3]);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_Y(node), expected_nodal_coordinates[i*3+1]);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_Z(node), expected_nodal_coordinates[i*3+2]);
    }

    for (int i=0; i<expected_number_of_elements; ++i) {
        CoSimIO_Element elem = CoSimIO_ModelPart_GetElementByIndex(model_part, i);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_Id(elem), i+1);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_Type(elem), CoSimIO_Triangle3D3);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_NumberOfNodes(elem), 3);
        // checking connectivities
        for (int j=0; j<CoSimIO_Element_NumberOfNodes(elem); ++j) {
            int exp_node_id = expected_elements_connectivities[i*3+j];
            COSIMIO_CHECK_EQUAL_INT(exp_node_id, CoSimIO_Node_Id(CoSimIO_Element_GetNodeByIndex(elem, j)));
        }
    }

    // Disconnecting at the end
    CoSimIO_Info disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
