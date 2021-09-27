# Tutorial for connecting to Kratos and performing basic data exchange

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Connecting to Kratos](#connecting-to-kratos)
- [Data Exchange with Kratos](#data-exchange-with-kratos)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial shows how to establish a connection to Kratos and to do basics data exchange with Kratos. It is required to do [this tutorial](integration_co_sim_io.md) first.

Furthermore it is required to compile Kratos which is described [here](../../kratos_build.md).

## Connecting to Kratos
Instead of connecting two instances of the same code as shown in the [previous tutorial](integration_co_sim_io.md#connecting-and-disconnecting), here one instance is connected to Kratos.
For this we use the same code from the previous tutorial.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#connecting-to-kratos).

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/connect_disconnect_c & python3 path/to/connect_disconnect.py
```

## Data Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#data-exchange-with-kratos).

On the other side, we use first export data and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#data-exchange):

```c
/* CoSimulation includes */
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    /* declaring variables */
    CoSimIO_Info connection_settings, connect_info, export_settings, import_settings, export_info, import_info, disconnect_settings, disconnect_info;
    const char* connection_name;
    int data_size = 4;
    double data_to_send[] = {3.14, 3.14, 3.14, 3.14};
    double* imported_data;
    int data_allocated_size = 0;

    /* Creating the connection settings */
    connection_settings = CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_export_data");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_import_data");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    /* Connecting using the connection settings */
    connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    /* After connecting we may export the data */

    /* Creating the export_settings */
    export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "vector_of_pi");
    CoSimIO_Info_SetString(export_settings, "connection_name", connection_name);

    /* Exporting the data */
    export_info = CoSimIO_ExportData(export_settings, data_size, data_to_send);
    /* Freeing the export_info and export_settings */
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_settings);

    /* importing the data back */

    /* Creating the import_settings */
    import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "vector_of_pi");
    CoSimIO_Info_SetString(import_settings, "connection_name", connection_name);

    /* Importing the data */
    import_info = CoSimIO_ImportData(import_settings, &data_allocated_size, &imported_data);
    /* Freeing the import_info and import_settings */
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    /* Freeing the data using CoSimIO_Free. (Not the standard free()) */
    CoSimIO_Free(imported_data);

    /* Disconnecting at the end */
    disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    disconnect_info = CoSimIO_Disconnect(disconnect_settings); /* disconnect afterwards */
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    /* Don't forget to release the settings and info */
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info); /* Don't forget to free the connect_info */
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
```

This file can be found in [integration_tutorials/c/data_exchange.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/data_exchange.c)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/data_exchange_c_test & python3 path/to/import_export_data.py
```

## Next steps
In the [next tutorial](mesh_exchange_with_kratos.md), meshes are exchanged with Kratos
