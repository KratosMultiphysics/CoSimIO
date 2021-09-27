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
settings.SetString("my_name", "my_code")
settings.SetString("connect_to", "Kratos")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = CoSimIO.Connect(settings)
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

# Exporting mesh to Kratos
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", connection_name)
return_info = CoSimIO.ExportMesh(info, model_part)

# Importing the same mesh from Kratos
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", connection_name)

model_part_import = CoSimIO.ModelPart("mp_import")
return_info = CoSimIO.ImportMesh(info, model_part_import)

# Checking the imported mesh
expected_nodal_coords = [
    (0.0, 2.5, 1.0),
    (2.0, 0.0, 1.5),
    (2.0, 2.5, 1.5),
    (4.0, 2.5, 1.7),
    (4.0, 0.0, 1.7),
    (6.0, 0.0, 1.8)
]

expected_element_connectivities = [
    (1, 2, 3),
    (2, 4, 3),
    (2, 5, 4),
    (4, 5, 6),
]

cosimio_check_equal(model_part_import.NumberOfNodes(), len(expected_nodal_coords))
cosimio_check_equal(model_part_import.NumberOfElements(), len(expected_element_connectivities))

for i, (coords, node) in enumerate(zip(expected_nodal_coords, model_part_import.Nodes)):
    cosimio_check_equal(i+1, node.Id())
    cosimio_check_equal(coords[0], node.X())
    cosimio_check_equal(coords[1], node.Y())
    cosimio_check_equal(coords[2], node.Z())

for i, (conn, elem) in enumerate(zip(expected_element_connectivities, model_part_import.Elements)):
    cosimio_check_equal(i+1, elem.Id())
    cosimio_check_equal(elem.Type(), CoSimIO.ElementType.Triangle3D3)
    cosimio_check_equal(elem.NumberOfNodes(), 3)
    for j, node in enumerate(elem.Nodes):
        cosimio_check_equal(node.Id(), conn[j])

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
