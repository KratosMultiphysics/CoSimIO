# Tutorial for connecting to Kratos and performing basic data exchange

This tutorial shows how to establish a connection to Kratos and to do basics data exchange with Kratos. It is required to do [this tutorial](integration_co_sim_io.md) first.

Furthermore it is required to compile Kratos which is described [here](../../kratos_build.md).

## Connecting to Kratos
Instead of connecting two instances of the same code as shown in [this tutorial](../cpp/integration_co_sim_io.md#connecting-and-disconnecting), here one instance is connected to Kratos.
For this we use the same code from the previous tutorial.

For connecting to Kratos it is very important to have in mind that Kratos also uses _CoSimIO_, so its python interface reflects the API of the _CoSimIO_. So we may create a python script for connecting and disconnecting in a similar way described in the [python tutorial](../python/integration_co_sim_io.md):

```py
from KratosMultiphysics.CoSimulationApplication import CoSimIO

connection_settings = CoSimIO.Info()
connection_settings.SetString("my_name", "Kratos")
connection_settings.SetString("connect_to", "my_code")
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

Make sure to check that `my_name` and `connect_to` are set correctly, otherwise the connection can not be established. This was explained in more detail in a [previous tutorial](integration_co_sim_io.md#connecting-and-disconnecting).

Please note that the only change here is the import statement which loads the _CoSimIO_ module which comes inside the KratosMultiphysics. You may find this python file in [here](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/connect_disconnect.py)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/tests_cpp/connect_disconnect_cpp_test & python3 path/to/connect_disconnect.py
```

## Data Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same. Again the python file for Kratos side is very similar to the one descirbed in the [python tutorial](../python/integration_co_sim_io.md):

```python
from KratosMultiphysics.CoSimulationApplication import CoSimIO

connection_settings = CoSimIO.Info()
connection_settings.SetString("my_name", "Kratos")
connection_settings.SetString("connect_to", "my_code")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
connection_name = info.GetString("connection_name")
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

# first the data from "my_code" is received ...
import_info = CoSimIO.Info()
import_info.SetString("connection_name", connection_name)
import_info.SetString("identifier", "data_exchange_1")
imported_values = CoSimIO.DoubleVector()
CoSimIO.ImportData(import_info, imported_values)

# print(imported_values)

# ... then the data is sent back to "my_code"
export_info = CoSimIO.Info()
export_info.SetString("connection_name", connection_name)
export_info.SetString("identifier", "data_exchange_2")
CoSimIO.ExportData(export_info, imported_values)

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")
```

You may find this python file in [here](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_data.py)

On the other side we use first export data and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#data-exchange):

```c++
// System includes
#include <cmath> // std::abs

// CoSimulation includes
#include "co_sim_io.hpp"

int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "my_code");
    settings.Set("connect_to", "Kratos");
    settings.Set("echo_level", 1);

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    // send data to Kratos
    std::vector<double> data_to_send(4, 3.14);
    info.Clear();
    info.Set("identifier", "vector_of_pi");
    info.Set("connection_name", connection_name);
    info = CoSimIO::ExportData(info, data_to_send);

    // receive the same data from Kratos
    std::vector<double> data_received;
    info.Clear();
    info.Set("identifier", "vector_of_pi");
    info.Set("connection_name", connection_name);
    info = CoSimIO::ImportData(info, data_received);

    // check that the data is the same:
    if (data_to_send.size() != data_received.size()) {
        return 1;
    }

    for (std::size_t i=0; i<data_to_send.size(); ++i) {
        // compare if values are the same, with a small tolerance
        if (std::abs(data_to_send[i] - data_received[i]) > 1e-12) {
            return 1;
        }
    }

    // disconnecting afterwards
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
```

This example can be found in [integration_tutorials/cpp/data_exchange.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/data_exchange.cpp)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/data_exchange_cpp_test & python3 path/to/connect_disconnect.py
```

## Next steps
In the [next tutorial](mesh_exchange_with_kratos.md), meshes are exchanged with Kratos
