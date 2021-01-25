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

def cosimio_check_equal(a, b):
    assert a == b


# Connection Settings
settings = CoSimIO.Info()
settings.SetString("my_name", "py_export_info")
settings.SetString("connect_to", "py_import_info")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = CoSimIO.Connect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
connection_name = return_info.GetString("connection_name")

# Exporting CoSimIO.Info
info_to_export = CoSimIO.Info()
info_to_export.SetString("connection_name", connection_name)
info_to_export.SetString("identifier", "info_exchange")
info_to_export.SetString("id", "convergence_information")
info_to_export.SetBool("is_converged", True)
info_to_export.SetDouble("tol", 0.008)
info_to_export.SetInt("echo_level", 2)
return_info = CoSimIO.ExportInfo(info_to_export)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
