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

def cosimio_check_equal(a, b):
    assert a == b

def cosimio_check_true(a):
    assert a

s_connection_name = ""

# auxiliar function for testing
def control_other_code(function_name):
    # we tell the other code what to do
    ctrl_info = CoSimIO.Info()
    ctrl_info.SetString("connection_name", s_connection_name)
    ctrl_info.SetString("identifier", "run_control")

    ctrl_info.SetString("control_signal", function_name)

    # give some additional information when doing Import/Export
    if (function_name == "ImportMesh" or
        function_name == "ExportMesh" or
        function_name == "ImportData" or
        function_name == "ExportData"):
        sub_settings = CoSimIO.Info()
        sub_settings.SetString("identifier", function_name)
        ctrl_info.SetInfo("settings", sub_settings) # must be named "settings"!

    CoSimIO.ExportInfo(ctrl_info) # here we tell the other code which function to call

    # this is for testing to make sure the function
    # we wanted to call was actually called
    import_info = CoSimIO.Info()
    import_info.SetString("connection_name", s_connection_name)
    import_info.SetString("identifier", "info_for_test")
    check_info = CoSimIO.ImportInfo(import_info)

    if (function_name == "ImportMesh" or
        function_name == "ExportMesh" or
        function_name == "ImportData" or
        function_name == "ExportData"):
        if (not check_info.Has("identifier_control")):
            print("ERROR: runner.py: missing \"identifier_control\"!")
            return False

        if (check_info.GetString("identifier_control") != function_name):
            print("ERROR: runner.py: wrong \"identifier_control\"! Expected:", function_name, ", got:", check_info.GetString("identifier_control"))
            return False

    return check_info.GetString("name_for_check") == function_name


# Connection Settings
settings = CoSimIO.Info()
settings.SetString("my_name", "py_runner")
settings.SetString("connect_to", "py_run")
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")

# Connecting
return_info = CoSimIO.Connect(settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Connected)
s_connection_name = return_info.GetString("connection_name")

# we tell the other code what to do

# initial mesh exchange
cosimio_check_true(control_other_code("ExportMesh"))
cosimio_check_true(control_other_code("ImportMesh"))
cosimio_check_true(control_other_code("ExportMesh"))
cosimio_check_true(control_other_code("ImportMesh"))

# "solution loop"
num_time_steps = 5
inner_iterations = 3
for _ in range(num_time_steps):
    cosimio_check_true(control_other_code("AdvanceInTime"))
    cosimio_check_true(control_other_code("InitializeSolutionStep"))
    cosimio_check_true(control_other_code("Predict"))
    for _ in range(inner_iterations):
        # importing twice, but exporting only once
        cosimio_check_true(control_other_code("ImportData"))
        cosimio_check_true(control_other_code("ImportData"))
        cosimio_check_true(control_other_code("SolveSolutionStep"))
        cosimio_check_true(control_other_code("ExportData"))
    cosimio_check_true(control_other_code("FinalizeSolutionStep"))
    cosimio_check_true(control_other_code("OutputSolutionStep"))

# tell the other code to exit the Run function
ctrl_info = CoSimIO.Info()
ctrl_info.SetString("connection_name", s_connection_name)
ctrl_info.SetString("identifier", "run_control")
ctrl_info.SetString("control_signal", "exit")
CoSimIO.ExportInfo(ctrl_info)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", s_connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
