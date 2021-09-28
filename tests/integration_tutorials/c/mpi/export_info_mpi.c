/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Philipp Bucher (https://github.com/philbucher)
*/

/* External includes */
#include "mpi.h"

/* CoSimulation includes */
#include "c/co_sim_io_c_mpi.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main(int argc, char** argv)
{
    /* declaring variables */
    CoSimIO_Info connection_settings, connect_info, info_to_export, export_info, disconnect_settings, disconnect_info;
    const char* connection_name;

    MPI_Init(&argc, &argv); /* needs to be done before calling CoSimIO_ConnectMPI */

    /* Creating the connection settings */
    connection_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(connection_settings, "my_name", "c_export_info");
    CoSimIO_Info_SetString(connection_settings, "connect_to", "c_import_info");
    CoSimIO_Info_SetInt(connection_settings, "echo_level", 1);
    CoSimIO_Info_SetString(connection_settings, "version", "1.25");

    /* Connecting using the connection settings */
    connect_info = CoSimIO_ConnectMPI(connection_settings, MPI_COMM_WORLD);
    COSIMIO_CHECK_EQUAL(CoSimIO_Info_GetInt(connect_info, "connection_status"), CoSimIO_Connected);
    connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    /* Creatinf the info to export */
    info_to_export=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(info_to_export, "connection_name", connection_name);
    CoSimIO_Info_SetString(info_to_export, "identifier", "c_info_exchange");
    CoSimIO_Info_SetString(info_to_export, "id", "convergence_information");
    CoSimIO_Info_SetBool(info_to_export, "is_converged", 1);
    CoSimIO_Info_SetDouble(info_to_export, "tol", 0.23);
    CoSimIO_Info_SetInt(info_to_export, "echo_level", 2);

    /* Exporting the Info */
    export_info = CoSimIO_ExportInfo(info_to_export);
    /* Freeing the export_info and info_to_export */
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(info_to_export);

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

    MPI_Finalize();

    return 0;
}
