#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Philipp Bucher
#

import CoSimIO
from CoSimIO.mpi import ConnectMPI
from mpi4py import MPI # this initializes MPI (calls MPI_Init)
from CoSimIO.mpi.mpi4pyInterface import mpi4pyCommHolder

def cosimio_check_equal(a, b):
    assert a == b


# Connection Settings
settings = CoSimIO.Info()
settings.SetString("my_name", "py_import_info")
settings.SetString("connect_to", "py_export_info")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = ConnectMPI(settings, mpi4pyCommHolder(MPI.COMM_WORLD))
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = return_info.GetString("connection_name")

# Exporting CoSimIO.Info
info = CoSimIO.Info()
info.SetString("connection_name", connection_name)
info.SetString("identifier", "info_exchange")
imported_info = CoSimIO.ImportInfo(info)
cosimio_check_equal(imported_info.GetString("id"), "convergence_information")
cosimio_check_equal(imported_info.GetBool("is_converged"), True)
cosimio_check_equal(imported_info.GetDouble("tol"), 0.008)
cosimio_check_equal(imported_info.GetInt("echo_level"), 2)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
