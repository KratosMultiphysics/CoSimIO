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
    CoSimIO_Info_SetString(connection_settings, "connection_name", "im_exp_data"); // This should be the same in both side
    CoSimIO_Info_SetString(connection_settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "solver_version", "1.25");

    // Connecting using the connection settings
    CoSimIO_Info connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    CoSimIO_FreeInfo(connect_info); // Don't forget to free the connect_info 

    // After conneting we may export the data
    int data_size = 4;
    double data_to_send[] = {3, .1, .14, 3.14};

    // Creatint the export_settings 
    CoSimIO_Info export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "data_exchange_1");
    CoSimIO_Info_SetString(export_settings, "connection_name", "im_exp_data");

    // Exporting the data
    CoSimIO_Info export_info = CoSimIO_ExportData(export_settings, data_size, data_to_send);
    // Freeing the export_info and export_settings
    CoSimIO_FreeInfo(export_info); 
    CoSimIO_FreeInfo(export_settings);


    // After conneting we may import the data
    double* data;
    int data_allocated_size = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "data_exchange_2");
    CoSimIO_Info_SetString(import_settings, "connection_name", "im_exp_data");

    // Importing the data
    CoSimIO_Info import_info = CoSimIO_ImportData(import_settings, &data_allocated_size, &data);
    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);

    for(int i = 0 ; i < data_size ; i++)
        COSIMIO_CHECK_EQUAL(data_to_send[i], data[i]);


    // Disconnecting at the end
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(connection_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
