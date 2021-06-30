# Tutorial for using the Kratos mapping

This tutorial shows how to map data between (non matching) meshes with Kratos. This page shows the Kratos side, please refer to the corresponding tutorials for the application side, depending on the language.

In this tutorial we first send two meshes based on the same geometry but with different discretizations to Kratos. Those meshes are used as basis for the mapping. In Kratos teminology those are the origin and the destination.

After sending the meshes, data that is to be mapped is sent to Kratos. It is mapped and then sent back.

Note that the following tutorial contains only a subset of the mapping capabilities of Kratos. Check [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication/README.md) for a more detailed explanation.


## Preparing meshes
The first step is to send the origin and destination meshes for mapping between. On the Kratos side we import these meshes as `Kratos::ModelPart`s. Please note that you should create the origin and destination `ModelPart`s before, and then use the `ImportMesh` method to get the mesh:

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
```

On the Kratos side, we need to allocate memory for the data and store it as nodal data in the imported origin `ModelPart`. For doing that, first we should add those nodal data to the `ModelPart` using the `AddNodalSolutionStepVariable` method. For example let's consider that we are receiving the temperature and we want to map it into the ambient temperature:

```py
## allocate memory
# scalar
model_part_origin.AddNodalSolutionStepVariable(KM.TEMPERATURE)
model_part_destination.AddNodalSolutionStepVariable(KM.AMBIENT_TEMPERATURE)

# vector
model_part_origin.AddNodalSolutionStepVariable(KM.VELOCITY)
model_part_destination.AddNodalSolutionStepVariable(KM.MESH_VELOCITY)
```

Please note that above methods must be called before importing the mesh. For the list of the variables which are defined in the core of Kratos and their types, please check the [variables.h](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/variables.h)


## Importing meshes
After the `ModelPart`s are prepared, they can be imported with the `ImportMesh` function (on the application side `ExportMesh` has to be used):

```py
import_mesh_info_o = CoSimIO.Info()
import_mesh_info_o.SetString("connection_name", connection_name)
import_mesh_info_o.SetString("identifier", "mesh_origin")
CoSimIO.ImportMesh(import_mesh_info_o, model_part_origin)

import_mesh_info_d = CoSimIO.Info()
import_mesh_info_d.SetString("connection_name", connection_name)
import_mesh_info_d.SetString("identifier", "mesh_destination")
CoSimIO.ImportMesh(import_mesh_info_d, model_part_destination)
```


## Importing data
After preparing the `ModelPart`s we can import the data from the application (to be mapped afterwards) and store it in the origin `ModelPart` using the `ImportData` method. From application side we will use the `CoSimIO::ExportData` function. For example for storing the data as temperature in origin `ModelPart` we will have:

```py
# import data to be mapped
import_data_info = CoSimIO.Info()
import_data_info.SetString("connection_name", connection_name)
import_data_info.SetString("identifier", "data_to_map")
CoSimIO.ImportData(import_data_info, model_part_origin, KM.TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Please note that here the `CoSimIO.DataLocation.NodeHistorical` argument is saying that we want to store the imported data as historical value in the nodes of this `ModelPart`. So the size of the data should be coherent with the number of the nodes in the `ModelPart`s.


## Mapping
Now it is time to do the mapping. The first step would be to import the mapping application which provides the mapping mechanism:

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
Here we have only created a mapper with nearest neighbor mapping algorithm. For more options please check the [mapping application documentation](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/MappingApplication/README.md). After creating the mapper we can use it to map the data from one mesh to the other one using the `Map` method:

```py
# map scalar quantities
mapper.Map(KM.TEMPERATURE, KM.AMBIENT_TEMPERATURE)

# map vector quantities
mapper.Map(KM.VELOCITY, KM.MESH_VELOCITY)
```

## Exporting mapped data
And finally to send back the mapped data to the application from Kratos side we will use the `ExportData` method:

```py
# export mapped data
export_data_info = CoSimIO.Info()
export_data_info.SetString("connection_name", connection_name)
export_data_info.SetString("identifier", "mapped_data")
CoSimIO.ExportData(export_data_info, model_part_destination, KM.AMBIENT_TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Again here the `CoSimIO.DataLocation.NodeHistorical` arguments denotes that we take the `AMBIENT_TEMPERATURE` from each node historical data and export it.

In application side we should import the data using the `ImportData` method:

The complete script for this case can be found [here](mapping_with_kratos.py).
