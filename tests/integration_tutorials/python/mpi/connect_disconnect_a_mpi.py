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

def cosimio_check_equal(a, b):
    assert a == b


settings = CoSimIO.Info()
settings.SetString("my_name", "py_connect_disconnect_a")
settings.SetString("connect_to", "py_connect_disconnect_b")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

info = ConnectMPI(settings) # uses MPI_COMM_WORLD if no communicator is passed
cosimio_check_equal(info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = info.GetString("connection_name")

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)

