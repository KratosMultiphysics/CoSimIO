#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Philipp Bucher (https://github.com/philbucher)
#

import CoSimIO
from CoSimIO.mpi import ConnectMPI
from mpi4py import MPI # this initializes and finalizes MPI (calls MPI_Init and MPI_Finalize)
from CoSimIO.mpi.mpi4pyInterface import mpi4pyCommHolder

def cosimio_check_equal(a, b):
    assert a == b


# Connection Settings
settings = CoSimIO.Info()
settings.SetString("my_name", "py_export_data")
settings.SetString("connect_to", "py_import_data")
settings.SetString("communication_format", "pipe")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = ConnectMPI(settings, mpi4pyCommHolder(MPI.COMM_WORLD))
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = return_info.GetString("connection_name")

# Exporting data
data_to_be_send=CoSimIO.DoubleVector([3.14] * 4)
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
return_info = CoSimIO.ExportData(info, data_to_be_send)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
