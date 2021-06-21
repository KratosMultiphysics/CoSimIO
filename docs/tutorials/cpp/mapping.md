# Tutorial for using the Kratos mapping

This tutorial shows how to use the mapping of Kratos. It is required to do [this tutorial](mesh_exchange_with_kratos.md) first.

## Exchanging mesh and data with Kratos
This tutorial shows how to map data between (non matching) meshes with Kratos.

In this tutorial we first send two meshes based on the same geometry but with different discretizations to Kratos. Those meshes are used as basis for the mapping. In Kratos teminology those are the origin and the destination.

After sending the meshes, data that is to be mapped is sent to Kratos. It is mapped and then sent back.

Note that the following tutorial contains only a subset of the mapping capabilities of Kratos. Check [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication) for a more detailed explanation.

The first step is to send the origin and destination meshes for mapping between:

```c++
// Creating the export mesh settings
CoSimIO_Info export_mesh_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_origin");
CoSimIO_Info_SetString(export_mesh_settings, "connection_name", connection_name);

// Exporting the origin mesh
CoSimIO_Info export_info_o = CoSimIO_ExportMesh(export_mesh_settings
    , export_number_of_nodes_o*3, export_number_of_elements_o,      export_number_of_elements_connectivities_o
    , export_nodal_coordinates_o, export_elements_connectivities_o, export_elements_types_o);

// Exporting the destination mesh
CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_destination");
CoSimIO_Info export_info_d = CoSimIO_ExportMesh(export_mesh_settings
    , export_number_of_nodes_d*3, export_number_of_elements_d,      export_number_of_elements_connectivities_d
    , export_nodal_coordinates_d, export_elements_connectivities_d, export_elements_types_d);

// Free memory
CoSimIO_FreeInfo(export_info_o);
CoSimIO_FreeInfo(export_info_d);
CoSimIO_FreeInfo(export_mesh_settings);
```


On the Kratos side we import these meshes as `Kratos::ModelPart`s, similar to the previous tutorial. Please note that you should create the origin and destination model parts before and then use the `ImportMesh` method to get the mesh:

```py
# importing the Kratos library
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO

# create the Kratos ModelParts that contain the mesh
model = KM.Model()
model_part_origin = model.CreateModelPart("mp_origin")
model_part_destination = model.CreateModelPart("mp_destination")

# connect to CoSimIO
connection_settings = CoSimIO.Info()
connection_settings.SetString("my_name", "Kratos")
connection_settings.SetString("connect_to", "my_code")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
connection_name = info.GetString("connection_name")
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

# import meshes
import_mesh_info_o = CoSimIO.Info()
import_mesh_info_o.SetString("connection_name", connection_name)
import_mesh_info_o.SetString("identifier", "mesh_origin")
CoSimIO.ImportMesh(import_mesh_info_o, model_part_origin)

import_mesh_info_d = CoSimIO.Info()
import_mesh_info_d.SetString("connection_name", connection_name)
import_mesh_info_d.SetString("identifier", "mesh_destination")
CoSimIO.ImportMesh(import_mesh_info_d, model_part_destination)

```
After sending the meshes now we should send the data field in the origin (to be mapped afterward). From application side we will use the `CoSimIO::ExportData` function:

```c++
// Creating the export origin data settings
CoSimIO_Info export_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_data_settings, "identifier", "data_to_map");
CoSimIO_Info_SetString(export_data_settings, "connection_name", connection_name);

// Exporting the origin data
CoSimIO_Info export_info = CoSimIO_ExportData(export_data_settings, export_data_size, export_data);

// Free memory
CoSimIO_FreeInfo(export_data_settings);
CoSimIO_FreeInfo(export_info);
```

On Kratos side, we should take this data and store it as nodal data in the imported origin `ModelPart`. For doing that, first we should add those nodal data to the model part using the `AddNodalSolutionStepVariable` method. For example let's consider that we are receiving the temperature and we want to map it into the ambient temperature:

```py
# allocate memory
model_part_origin.AddNodalSolutionStepVariable(KM.TEMPERATURE)
model_part_destination.AddNodalSolutionStepVariable(KM.AMBIENT_TEMPERATURE)

model_part_origin.AddNodalSolutionStepVariable(KM.VELOCITY)
model_part_destination.AddNodalSolutionStepVariable(KM.MESH_VELOCITY)
```

Please note that above methods should be called before importing the mesh. For the list of the variables which are defined in the core of Kratos and their types, please check the [variables.h](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/variables.h)

After preparing the `ModelPart`s we can import the data from the application and store it in the origin model part using the `ImportData` method. For example for storing the data as temperature in origin model part we will have:

```py
# import data to be mapped
import_data_info = CoSimIO.Info()
import_data_info.SetString("connection_name", connection_name)
import_data_info.SetString("identifier", "data_to_map")
CoSimIO.ImportData(import_data_info, model_part_origin, KM.TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Please note that here the `CoSimIO.DataLocation.NodeHistorical` argument is saying that we want to store the imported data as historical value in the nodes of this model part. So the size of the data should be coherent with the number of the nodes in the model parts.

Now is time to do the mapping. The first step would be to import the mapping application which provides the mapping mechanism:

```py
import KratosMultiphysics.MappingApplication as KratosMapping
```

Then we can create a mapper using the mapper factory:

```py

# input for the mapper is a Kratos::Parameters object
mapper_settings = KM.Parameters("""{
    "mapper_type": "nearest_neighbor",
    "echo_level" : 0
}""")

# creating the mapper using the mapper factory
mapper = KratosMapping.MapperFactory.CreateMapper(
    model_part_origin,
    model_part_destination,
    mapper_settings)

```
Here we have only created a mapper with nearest neighbor mapping algorithm. For more options please check the [mapping application documentation](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication). After creating the mapper we can use it to map the data from one mesh to the other one using the `Map` method:

```py
# map scalar quantities
mapper.Map(KM.TEMPERATURE, KM.AMBIENT_TEMPERATURE)

# map vector quantities
mapper.Map(KM.VELOCITY, KM.MESH_VELOCITY)
```
And finally to send back the mapped data to the application from kratos side we will use the `ExportData` method:

```py
# export mapped data
export_data_info = CoSimIO.Info()
export_data_info.SetString("connection_name", connection_name)
export_data_info.SetString("identifier", "mapped_data")
CoSimIO.ExportData(export_data_info, model_part_destination, KM.AMBIENT_TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Again here the `CoSimIO.DataLocation.NodeHistorical` arguments denotes that we take the `AMBIENT_TEMPERATURE` from each node historical data and export it.

In application side we should import the data as described in data transfer tutorial:

```c++
// Import the mapped destination data
double* mapped_data;
int data_allocated_size = 0;

// Creating the import data settings
CoSimIO_Info import_mapped_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_mapped_data_settings, "identifier", "mapped_data");
CoSimIO_Info_SetString(import_mapped_data_settings, "connection_name", connection_name);

// Importing the data
CoSimIO_Info import_info = CoSimIO_ImportData(import_mapped_data_settings, &data_allocated_size, &mapped_data);

// Free memory
CoSimIO_FreeInfo(import_info);
CoSimIO_FreeInfo(import_mapped_data_settings);
```
This example can be found in [integration_tutorials/cpp/mesh_exchange.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/mesh_exchange.cpp)
