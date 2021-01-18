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
settings.SetString("my_name", "py_import_data")
settings.SetString("connect_to", "py_export_data")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = CoSimIO.Connect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = return_info.GetString("connection_name")

# # Importing data
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
vec_to_import=CoSimIO.DoubleVector()
return_info = CoSimIO.ImportData(info, vec_to_import)

cosimio_check_equal(len(vec_to_import), 4)

for value in vec_to_import:
    cosimio_check_equal(value, 3.14)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
