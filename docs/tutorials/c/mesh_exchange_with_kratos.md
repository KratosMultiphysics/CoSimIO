# Tutorial for exchanging meshes with Kratos

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Mesh Exchange with Kratos](#mesh-exchange-with-kratos)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial shows how to exchange meshes with Kratos. It is required to do [this tutorial](basic_data_exchange_with_kratos.md) first.

## Mesh Exchange with Kratos
Here we try to send the mesh (in the form of `CoSimIO::ModelPart`) to Kratos and get it back from it. Then we can check if both meshes are the same.
The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/mesh_exchange_with_kratos.md#mesh-exchange-with-kratos).

On the other side we use first export mesh and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#mesh-exchange):

```c
/* CoSimulation includes */
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
    /* declaring variables */
    int i,j;
    CoSimIO_Info connect_info, export_settings, import_settings, export_info, import_info, disconnect_settings, disconnect_info;
    const char* connection_name;
    CoSimIO_ModelPart model_part_export;
    CoSimIO_ModelPart model_part_import;
    CoSimIO_Node node;
    CoSimIO_Element elem;
    int number_of_nodes=6;
    double nodal_coordinates[] = {
        0.0, 2.5, 1.0, /*1*/
        2.0, 0.0, 1.5, /*2*/
        2.0, 2.5, 1.5, /*3*/
        4.0, 2.5, 1.7, /*4*/
        4.0, 0.0, 1.7, /*5*/
        6.0, 0.0, 1.8  /*6*/
    };

    int number_of_elements = 4;
    int elements_connectivities[] = {
        1, 2, 3, /*1*/
        2, 4, 3, /*2*/
        2, 5, 4, /*3*/
        4, 5, 6, /*4*/
    };

    const int num_nodes_per_element = 3;
    int connectivity[3];

    /* Creating the connection settings */
    CoSimIO_Info connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_export_mesh");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_import_mesh");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    /* Connecting using the connection settings */
    connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    model_part_export = CoSimIO_CreateModelPart("fluid_mesh");

    for (i=0; i<number_of_nodes; ++i) {
        CoSimIO_ModelPart_CreateNewNode(
            model_part_export,
            i+1, /* Id */
            nodal_coordinates[i*3],   /* X */
            nodal_coordinates[i*3+1], /* Y */
            nodal_coordinates[i*3+2]  /* Z */
        );
    }

    for (i=0; i<number_of_elements; ++i) {
        for (j=0; j<num_nodes_per_element; ++j) {
            connectivity[j] = elements_connectivities[i*3+j];
        }
        CoSimIO_ModelPart_CreateNewElement(
            model_part_export,
            i+1, /* Id */
            CoSimIO_Triangle3D3,
            connectivity
        );
    }

    /* Creating the export_settings */
    export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(export_settings, "connection_name", connection_name);

    /* Exporting the data */
    export_info = CoSimIO_ExportMesh(export_settings, model_part_export);

    /* Freeing the export_info and export_settings */
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_settings);

    /* After conneting we may import the mesh */
    model_part_import = CoSimIO_CreateModelPart("fluid_mesh");

    /* Creating the import_settings */
    import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
    CoSimIO_Info_SetString(import_settings, "connection_name", connection_name);

    /* Importing the mesh */
    import_info = CoSimIO_ImportMesh(import_settings, model_part_import);

    /* Freeing the import_info and import_settings */
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    /* Checking the imported mesh */
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_ModelPart_NumberOfNodes(model_part_import), number_of_nodes);
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_ModelPart_NumberOfElements(model_part_import), number_of_elements);

    for (i=0; i<number_of_nodes; ++i) {
        node = CoSimIO_ModelPart_GetNodeByIndex(model_part_import, i);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Node_Id(node), i+1);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_X(node), nodal_coordinates[i*3]);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_Y(node), nodal_coordinates[i*3+1]);
        COSIMIO_CHECK_EQUAL_DOUBLE(CoSimIO_Node_Z(node), nodal_coordinates[i*3+2]);
    }

    for (i=0; i<number_of_elements; ++i) {
        elem = CoSimIO_ModelPart_GetElementByIndex(model_part_import, i);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_Id(elem), i+1);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_Type(elem), CoSimIO_Triangle3D3);
        COSIMIO_CHECK_EQUAL_INT(CoSimIO_Element_NumberOfNodes(elem), 3);
        /* checking connectivities */
        for (j=0; j<CoSimIO_Element_NumberOfNodes(elem); ++j) {
            int exp_node_id = elements_connectivities[i*3+j];
            COSIMIO_CHECK_EQUAL_INT(exp_node_id, CoSimIO_Node_Id(CoSimIO_Element_GetNodeByIndex(elem, j)));
        }
    }

    /* Disconnecting at the end */
    disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    disconnect_info = CoSimIO_Disconnect(disconnect_settings); /* disconnect afterwards */
    COSIMIO_CHECK_EQUAL_INT(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    /* Don't forget to release the settings and info */
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info); /* Don't forget to free the connect_info */
    CoSimIO_FreeInfo(disconnect_info);
    CoSimIO_FreeModelPart(model_part_export);
    CoSimIO_FreeModelPart(model_part_import);

    return 0;
}
```

This file can be found in [integration_tutorials/c/mesh_exchange.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/mesh_exchange.c)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/mesh_exchange_c_test & python3 path/to/import_export_mesh.py
```

## Next steps
In the [next tutorial](mapping.md), meshes are exchanged with Kratos and mapping is used.
