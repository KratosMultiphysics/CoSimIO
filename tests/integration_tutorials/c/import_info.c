/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Pooyan Dadvand
*/

/* System includes */
#include <stdio.h>
#include <math.h>
#include <string.h>

/* CoSimulation includes */
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_INT_EQUAL(a, b)                                \
    if (a != b) {                                                    \
        printf("in line %d: %d is not equal to %d", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_DOUBLE_EQUAL(a, b)                             \
    if (fabs(a-b)>1e-15) {                                           \
        printf("in line %d: %f is not equal to %f", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_STRING_EQUAL(a, b)                             \
    if (strcmp(a,b)) {                                               \
        printf("in line %d: %s is not equal to %s", __LINE__, a, b); \
        return 1;                                                    \
    }

int main()
{
    /* declaring variables */
    CoSimIO_Info connection_settings, connect_info, export_info, imported_info, disconnect_settings, disconnect_info;
    const char* connection_name;

    /* Creating the connection settings */
    connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_import_info");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_export_info");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    /* Connecting using the connection settings */
    connect_info = CoSimIO_Connect(connection_settings);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    /* Creating the info for export */
    export_info=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_info, "connection_name", connection_name);
    CoSimIO_Info_SetString(export_info, "identifier", "c_info_exchange");

    /* import the info */
    imported_info = CoSimIO_ImportInfo(export_info);

    /* check imported Info */
    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_Info_GetString(imported_info, "id"), "convergence_information");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetBool(imported_info, "is_converged"), 1);
    COSIMIO_CHECK_DOUBLE_EQUAL(CoSimIO_Info_GetDouble(imported_info, "tol"), 0.23);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(imported_info, "echo_level"), 2);

    /* Freeing the export_info and imported_info */
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(imported_info);

    /* Disconnecting at the end */
    disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    disconnect_info = CoSimIO_Disconnect(disconnect_settings); /* disconnect afterwards */
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    /* Don't forget to release the settings and info */
    CoSimIO_FreeInfo(connection_settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info); /* Don't forget to free the connect_info */
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
