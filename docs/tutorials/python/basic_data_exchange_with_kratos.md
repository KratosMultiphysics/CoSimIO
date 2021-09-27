# Tutorial for connecting to Kratos and performing basic data exchange

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Connecting to Kratos](#connecting-to-kratos)
- [Data Exchange with Kratos](#data-exchange-with-kratos)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial shows how to establish a connection to Kratos and to do basics data exchange with Kratos. It is required to do [this tutorial](integration_co_sim_io.md) first.

Furthermore it is required to compile Kratos which is described [here](../../kratos_build.md).

## Connecting to Kratos
Instead of connecting two instances of the same code as shown in the [previous tutorial](integration_co_sim_io.md#connecting-and-disconnecting), here one instance is connected to Kratos.
For this we use the same code from the previous tutorial.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#connecting-to-kratos).

Then you may run your _CoSimIO_ python script with the python script of Kratos from your working directory:

```shell
python3 path/to/co_sim_io_connect_disconnect.py & python3 path/to/connect_disconnect.py
```

## Data Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same.

The Kratos side of this tutorial can be found in the [C++ tutorial](../cpp/basic_data_exchange_with_kratos.md#data-exchange-with-kratos).

On the other side, we use first export data and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#data-exchange):

```py
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

# send data to Kratos
data_to_be_send=CoSimIO.DoubleVector([3.14] * 4)
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
return_info = CoSimIO.ExportData(info, data_to_be_send)

# receive the same data from Kratos
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
vec_to_import=CoSimIO.DoubleVector()
return_info = CoSimIO.ImportData(info, vec_to_import)

# check that the data is the same:
cosimio_check_equal(len(vec_to_import), len(data_to_be_send))

for value, val_ref in zip(vec_to_import, data_to_be_send):
    cosimio_check_equal(value, val_ref)

# Disconnecting
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
return_info = CoSimIO.Disconnect(disconnect_settings)
cosimio_check_equal(return_info.GetInt("connection_status"), CoSimIO.ConnectionStatus.Disconnected)
```

This file can be found in [integration_tutorials/python/data_exchange.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/data_exchange.py)

Then you may run your _CoSimIO_ python script with the python script of Kratos from your working directory:

```shell
python3 path/to/data_exchange.py & python3 path/to/import_export_data.py
```

## Next steps
In the [next tutorial](mesh_exchange_with_kratos.md), meshes are exchanged with Kratos
