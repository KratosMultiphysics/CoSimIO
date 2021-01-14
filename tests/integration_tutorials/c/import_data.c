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
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_import_data");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_export_data");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    // After conneting we may import the data
    double* data;
    int data_allocated_size = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "vector_of_pi");
    CoSimIO_Info_SetString(import_settings, "connection_name", connection_name);

    // Importing the data
    CoSimIO_Info import_info = CoSimIO_ImportData(import_settings, &data_allocated_size, &data);
    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    // Freeing the data using CoSimIO_Free. (Not the standard free())
    CoSimIO_Free(data);

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