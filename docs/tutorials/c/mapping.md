# Tutorial for using the Kratos mapping

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

This tutorial shows how to use the mapping of Kratos. It is required to do [this tutorial](mesh_exchange_with_kratos.md) first.

This page shows the application side, please see [here](../kratos_mapping.md) for the Kratos side.

The first step is to establish a connection with Kratos:

```c
CoSimIO_Info settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(settings, "my_name", "my_code");
CoSimIO_Info_SetString(settings, "connect_to", "Kratos");
CoSimIO_Info_SetInt(settings, "echo_level", 1);
CoSimIO_Info_SetString(settings, "version", "1.25");

CoSimIO_Info connect_info = CoSimIO_Connect(settings);
CoSimIO_FreeInfo(settings);

const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name"); // getting name of connection for future calls

if (CoSimIO_Info_GetInt(connect_info, "connection_status") != CoSimIO_Connected)
    return 1;
```

After establishing the connection, the two meshes (origin and destination) for mapping are sent to Kratos:

```c
// send origin mesh
CoSimIO_Info_SetString(info, "identifier", "mesh_origin");
CoSimIO_Info_SetString(info, "connection_name", connection_name);
CoSimIO_Info info_export_o = ExportMesh(info, model_part_origin);

// send destination mesh
CoSimIO_Info_SetString(info, "identifier", "mesh_destination");
CoSimIO_Info_SetString(info, "connection_name", connection_name);
CoSimIO_Info info_export_d = ExportMesh(info, model_part_destination);
```

Now the data of the origin mesh is sent to Kratos, where it is mapped to the destination mesh:

```c
// send origin data
CoSimIO_Info_SetString(info, "identifier", "data_to_map");
CoSimIO_Info_SetString(info, "connection_name", connection_name);
CoSimIO_Info export_data_info = CoSimIO_ExportData(info, data_size, data_to_map);
```
Note that the number of nodes has to correlate with the size of the data. For scalar data the size has to be the same as for the same as the number of nodes. For vectorial data, it has to be n-times the number of nodes, where n is the dimension of the vector (e.g. 3 for 3-dimensional data). The order of vectorial data is: [x1, y1, z1, x2, y2, z2, ...].

Now the mapping happens in Kratos and the final step is to receive the mapped values:

```c
// receive (mapped) destination data
CoSimIO_Info_SetString(info, "identifier", "mapped_data");
CoSimIO_Info_SetString(info, "connection_name", connection_name);
CoSimIO_Info export_data_info = CoSimIO_ImportData(info, &data_size, &mapped_data);
```
