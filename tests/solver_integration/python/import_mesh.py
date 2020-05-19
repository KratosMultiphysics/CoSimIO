#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Pooyan Dadvand
#

import CoSimIO

def cosimio_check_equal(a, b):
    assert a == b

# Connection Settings
settings = CoSimIO.Info()
settings.SetString("connection_name", "test_connection") # This must be unique for each connection between two solvers
settings.SetString("solver_name", "my_solver") # Not to be confused with the connection name.
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

# Connecting
return_info = CoSimIO.Connect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)



# Importing mesh
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", "test_connection")
return_info, nodal_coords, element_connectivities, element_types = CoSimIO.ImportMesh(info)

# Checking the imported mesh
expected_nodal_coords = [
    0.0, 2.5, 1.0, # 0
    2.0, 0.0, 1.5, # 1
    2.0, 2.5, 1.5, # 2
    4.0, 2.5, 1.7, # 3
    4.0, 0.0, 1.7, # 4
    6.0, 0.0, 1.8  # 5
]

expected_element_connectivities = [
    0, 1, 2, # 1
    1, 3, 2, # 2
    1, 4, 3, # 3
    3, 4, 5, # 4
]

expected_element_types = [
    5,5,5,5 # VTK_TRIANGLE
]

cosimio_check_equal(nodal_coords, expected_nodal_coords)
cosimio_check_equal(element_connectivities, expected_element_connectivities)
cosimio_check_equal(element_types, element_types)

# Disconnecting
return_info = CoSimIO.Disconnect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)

