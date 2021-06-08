# Tutorial for connecting to Kratos and performing basic data exchange

This tutorial shows how to establish a connection to Kratos and to do basics data exchange with Kratos. It is required to do [this tutorial](integration_co_sim_io.md) first

## Connecting to Kratos
For connecting to Kratos it is very important to have in mind that Kratos also uses _CoSimIO_, so its python interface reflects the _CoSimIO_. So we may create a python script for connecting and disconnecting in a similar way described in the [python tutorial](../python/integration_co_sim_io.md):


```py
from KratosMultiphysics.CoSimulationApplication import CoSimIO

connection_settings = CoSimIO.Info()
connection_settings.SetString("my_name", "partner_b")
connection_settings.SetString("connect_to", "partner_a")
connection_settings.SetInt("echo_level", 2)
info = CoSimIO.Connect(connection_settings)
connection_name = info.GetString("connection_name")
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")
```

Please note that the only change here is the import statement which loads the _CoSimIO_ module which comes inside the KratosMultiphysics. You may find this python file in [here](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/connect_disconnect.py)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/tests_cpp/connect_disconnect_cpp_test & python3 path/to/connect_disconnect.py
```
<!--
## Tutorial 9: Data Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same. Again the python file for Kratos side is very similar to the one descirbed in the [python tutorial](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tutorial/python/README.md):


```python
from KratosMultiphysics.CoSimulationApplication import CoSimIO

connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "im_exp_data")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

import_info = CoSimIO.Info()
import_info.SetString("connection_name", "im_exp_data")
import_info.SetString("identifier", "data_exchange_1")
imported_values = CoSimIO.ImportData(import_info)

# print(imported_values)

export_info = CoSimIO.Info()
export_info.SetString("connection_name", "im_exp_data")
export_info.SetString("identifier", "data_exchange_2")
CoSimIO.ExportData(export_info, imported_values)

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", "im_exp_data")

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")
```

-->