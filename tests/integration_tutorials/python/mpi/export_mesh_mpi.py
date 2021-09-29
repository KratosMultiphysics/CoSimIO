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
from mpi4py import MPI # this initializes MPI (calls MPI_Init)
from CoSimIO.mpi.mpi4pyInterface import mpi4pyCommHolder

def cosimio_check_equal(a, b):
    assert a == b

# Connection Settings
settings = CoSimIO.Info()
settings.SetString("my_name", "py_mesh_export_solver")
settings.SetString("connect_to", "py_mesh_import_solver")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = ConnectMPI(settings, mpi4pyCommHolder(MPI.COMM_WORLD))
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = return_info.GetString("connection_name")

# Creating the mesh
nodal_coords = [
    (0.0, 2.5, 1.0),
    (2.0, 0.0, 1.5),
    (2.0, 2.5, 1.5),
    (4.0, 2.5, 1.7),
    (4.0, 0.0, 1.7),
    (6.0, 0.0, 1.8)
]

element_connectivities = [
    (1, 2, 3),
    (2, 4, 3),
    (2, 5, 4),
    (4, 5, 6),
]

model_part = CoSimIO.ModelPart("mp_exchange")

for i, coords in enumerate(nodal_coords):
    model_part.CreateNewNode(i+1, *coords)

for i, conn in enumerate(element_connectivities):
    model_part.CreateNewElement(i+1, CoSimIO.ElementType.Triangle3D3, conn)

# Exporting mesh
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", connection_name)
return_info = CoSimIO.ExportMesh(info, model_part)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
