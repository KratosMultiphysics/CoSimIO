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
    CoSimIO_Info settings, connect_info, disconnect_settings, disconnect_info;
    const char* connection_name;

    (void)(argc);
    (void)(argv);

    settings = CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "my_name", "c_connect_disconnect_b");
    CoSimIO_Info_SetString(settings, "connect_to", "c_connect_disconnect_a");
    CoSimIO_Info_SetInt(settings, "echo_level", 1);
    CoSimIO_Info_SetString(settings, "version", "1.25");

    /* The connect must be called before any CosimIO function called */
    connect_info = CoSimIO_Connect(settings);
    connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);

    /* Now you may call any CoSimIO functions*/

    /* ... */

    /* You should free the info before getting a new one */
    disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(disconnect_settings, "connection_name", connection_name);
    disconnect_info = CoSimIO_Disconnect(disconnect_settings); /* disconnect afterwards */
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(disconnect_info, "connection_status"), CoSimIO_Disconnected);

    /* Don't forget to release the settings and info */
    CoSimIO_FreeInfo(settings);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(connect_info);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
