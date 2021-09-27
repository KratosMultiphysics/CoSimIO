# Tutorial for using the Kratos mapping

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

This tutorial shows how to use the mapping of Kratos. It is required to do [this tutorial](mesh_exchange_with_kratos.md) first.

This page shows the application side, please see [here](../kratos_mapping.md) for the Kratos side.

The first step is to establish a connection with Kratos:

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
```

After establishing the connection, the two meshes (origin and destination) for mapping are sent to Kratos:

```py
# send origin mesh
info.Set("identifier", "mesh_origin")
info.Set("connection_name", connection_name)
info = CoSimIO.ExportMesh(info, model_part_origin)

# send destination mesh
info.Set("identifier", "mesh_destination")
info.Set("connection_name", connection_name)
info = CoSimIO.ExportMesh(info, model_part_destination)
```

Now the data of the origin mesh is sent to Kratos, where it is mapped to the destination mesh:

```py
# send origin data
info.Set("identifier", "data_to_map")
info.Set("connection_name", connection_name)
info = CoSimIO.ExportData(info, data_to_map)
```
Note that the number of nodes has to correlate with the size of the data. For scalar data the size has to be the same as for the same as the number of nodes. For vectorial data, it has to be n-times the number of nodes, where n is the dimension of the vector (e.g. 3 for 3-dimensional data). The order of vectorial data is: [x1, y1, z1, x2, y2, z2, ...].

Now the mapping happens in Kratos and the final step is to receive the mapped values:

```py
# receive (mapped) destination data
info.Set("identifier", "mapped_data")
info.Set("connection_name", connection_name)
info = CoSimIO.ImportData(info, mapped_data)
```
