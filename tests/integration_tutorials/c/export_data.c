/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Pooyan Dadvand
*/

/* CoSimulation includes */
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main(int argc, const char** argv)
{
    /* declaring variables */
    CoSimIO_Info connection_settings, connect_info, export_settings, export_info, disconnect_settings, disconnect_info;
    const char* connection_name;
    int data_size = 4;
    double data_to_send[] = {3.14, 3.14, 3.14, 3.14};

    (void)(argc);
    (void)(argv);

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
