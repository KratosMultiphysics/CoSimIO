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


settings = CoSimIO.Info()
settings.SetString("connection_name", "test_connection") # This must be unique for each connection between two solvers
settings.SetString("solver_name", "my_solver") # Not to be confused with the connection name.
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

info = CoSimIO.Connect(settings)
cosimio_check_equal(info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)

info = CoSimIO.Disconnect(settings)
cosimio_check_equal(info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)

