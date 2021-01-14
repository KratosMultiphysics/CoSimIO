# Tutorial for integrating the _CoSimIO_ using the C interface

This tutorial helps you through to integrate the _CoSimIO_ into a solver/software-tool using the C interface.

## Overview

- [Tutorial for integrating the _CoSimIO_ using the C interface](#tutorial-for-integrating-the-cosimio-using-the-c-interface)
  - [Overview](#overview)
  - [What you need](#what-you-need)
  - [Tutorial 1: Building the CoSimIO](#tutorial-1-building-the-cosimio)
  - [Tutorial 2: Hello CosimIO](#tutorial-2-hello-cosimio)
  - [Tutorial 3: Connecting and Disconnecting](#tutorial-3-connecting-and-disconnecting)
  - [Tutorial 4: Data Exchange](#tutorial-4-data-exchange)
  - [Tutorial 5: Mesh Exchange](#tutorial-5-mesh-exchange)
  - [Tutorial 6: Kratos CoSimulation Library Overview](#tutorial-6-kratos-cosimulation-library-overview)
  - [Tutorial 7: Building Kratos with CoSimulation](#tutorial-7-building-kratos-with-cosimulation)
  - [Tutorial 8: Connecting/Disconnecting to/from Kratos](#tutorial-8-connectingdisconnecting-tofrom-kratos)
  - [Tutorial 9: Data Exchange with Kratos](#tutorial-9-data-exchange-with-kratos)
  - [Tutorial 10: Mesh Exchange with Kratos](#tutorial-10-mesh-exchange-with-kratos)
  - [Tutorial 11: Mapping with Kratos](#tutorial-11-mapping-with-kratos)

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler because the library by itself is in C++ and only a small C wrapper provides its C interface. most of the major C++ compilers in Windows, Linux, and Mac are C++11 compatible. You may find a detailed list of C++11 compatible compilers in https://en.cppreference.com/w/cpp/compiler_support#cpp11


## Tutorial 1: Building the CoSimIO
The C version of CosimIO is implemented in the [co_sim_io_c.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/c/co_sim_io_c.cpp). You may include it directly into your project or compiling it as a library. One may use the [build_c.sh]() script from the CoSimIO root folder to create a shared library of the C interface:

```bash
$ bash scripts/build_c.sh
```

The shared library will be installed in `bin/` folder. After building and linking it to your project, you may use the interface defined in `co_sim_io_c.h`:

```c
// CoSimulation includes
#include "c/co_sim_io_c.h"

int main(){
    return 0;
}
```

Please don't forget to add the CoSimIO folder to your include path.


## Tutorial 2: Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```c
CoSimIO_Info hello_info = CoSimIO_Hello();
```

Please note that this function like other functions in _CoSimIO_ returns an `Info` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be printed using `CoSimIO_PrintInfo()` function:

```c
CoSimIO_PrintInfo(stdout, hello_info);
```

You may retriving a certain value from the info object using provided functions for each value type:

```c
int major_version = CoSimIO_Info_GetInt(hello_info, "major_version");
int minor_version = CoSimIO_Info_GetInt(hello_info, "minor_version");
const char* patch_version = CoSimIO_Info_GetString(hello_info, "patch_version");
```

Finally it is **very important** to free the info using the `CoSimIO_FreeInfo()` function:

```c
CoSimIO_FreeInfo(hello_info);
```

This example can be found in [integration_tutorials/c/hello.c](../../tests/integration_tutorials/c/hello.c).


## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `CoSimIO_Connect()` function:
```c
// The connect must be called before any CosimIO function called
CoSimIO_Info connect_info = CoSimIO_Connect(settings);
```

First of all, you may notice that `Connect()` function takes an `Info` as its arguments. So first you should create it using `CoSimIO_CreateInfo()` function:

```c
CoSimIO_Info settings = CoSimIO_CreateInfo();
```

It is important to mention that the `CoSimIO_Info` is a pointer to the cpp info class which is allocated by the `CoSimIO_CreateInfo()`. So it is don't forget to free it when is not needed anymore using `CoSimIO_FreeInfo()` function. This container can be used to pass additional information about the solver/software-tool or connection settings to the CoSimIO:

```c
CoSimIO_Info_SetString(settings, "my_name", "the_name_of_this_solver"); // The name of this solver
CoSimIO_Info_SetString(settings, "connect_to", "the_other_solver_name"); // The name of the solver to connect to
CoSimIO_Info_SetInt(settings, "echo_level", 1);
CoSimIO_Info_SetString(settings, "solver_version", "1.25");
```
This function returns a `Info` object containing information about the connection which can be queried using `CoSimIO_Info_Get...` functions:

```c
CoSimIO_Info_GetString(info, "connection_name");
CoSimIO_Info_GetInt(info, "connection_status");
```

Now putting together everything:

```c
// CoSimulation includes
#include "c/co_sim_io_c.h"

int main()
{
    CoSimIO_Info settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "my_name", "the_name_of_this_solver"); // The name of this solver
    CoSimIO_Info_SetString(settings, "connect_to", "the_other_solver_name"); // The name of the solver to connect to
    CoSimIO_Info_SetInt(settings, "echo_level", 1);
    CoSimIO_Info_SetString(settings, "solver_version", "1.25");

    // The connect must be called before any CosimIO function called
    CoSimIO_Info connect_info = CoSimIO_Connect(settings);
    CoSimIO_FreeInfo(settings);

    // The connect_info contains now:
    // - The name of the connection ("connection_name") to be used for further calls to CoSimIO
    // - The status of the connection ("connection_status")

    const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name");

    if (CoSimIO_Info_GetInt(connect_info, "connection_status") != CoSimIO_Connected)
        return 1;

    // Don't forget to release the connect_info after getting your information
    CoSimIO_FreeInfo(connect_info);

    // Now you may call any CoSimIO functions
    // ...

    // Here you may use the info but cannot call any CoSimIO function anymore
    CoSimIO_Info disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "connection_name", connection_name);
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(disconnect_settings); // disconnect afterwards
    if(CoSimIO_Info_GetInt(disconnect_info, "connection_status") != CoSimIO_Disconnected)
        return 1;

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
```

This example can be found in [integration_tutorials/c/connect_disconnect.c](../../tests/integration_tutorials/c/connect_disconnect.c).


## Tutorial 4: Data Exchange
One of the important missions of the CoSimIO is to send and recieve data between processes. The `CoSimIO_ExportData()` function can be used to send data to the Kratos or directly to another solver/software-tool. For exporting the following array 4 doubles:

```c
double data_to_send[] = {3.14, 3.14, 3.14, 3.14};
```
First we should create a setting which provides an identifier (like "velocity_of_structure") and the connection name:

```c
// Creatint the export_settings
CoSimIO_Info export_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_settings, "identifier", "vector_of_pi");
CoSimIO_Info_SetString(export_settings, "connection_name", "test_connection");
```
And then use it to export the data:
```c
// Exporting the data
CoSimIO_Info export_info = CoSimIO_ExportData(export_settings, data_size, data_to_send);
```
Please note that the `data_size` parameter is number of entitities in array. (Is not the size in bytes)

The `ImportData()` should be used on the other side to recieve data:

```c
// Importing the data
CoSimIO_Info import_info = CoSimIO_ImportData(import_settings, &data_allocated_size, &data);
```
For the above example the settings and arguments are:

```c
double* data;
int data_allocated_size = 0;

// Creatint the import_settings
CoSimIO_Info import_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "vector_of_pi");
CoSimIO_Info_SetString(import_settings, "connection_name", "test_connection");
```

In this case we just pass an empty pointer and specifying to the `ImportData()` that should allocate the data by itself. So, in order to ensure the memory coherance, the `CoSimIO_Free()` function should be used instead of standard `free()` function:

```c
// Freeing the data using CoSimIO_Free. (Not the standard free())
CoSimIO_Free(data);
```
You may also allocate the memory for data by `CoSimIO_Malloc()` function. If the allocated size is larger that imported data then there is no reallocation is done but if imported data is larger, then only if the data is allocated by `CoSimIO_Malloc()` function a reallocation will be done. If not, it will gives an error.

This example can be found in [integration_tutorials/c/export_data.c](../../tests/integration_tutorials/c/export_data.c) and [integration_tutorials/c/import_data.c](../../tests/integration_tutorials/c/import_data.c).


## Tutorial 5: Mesh Exchange
After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```c
CoSimIO_Info export_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_settings, "identifier", "fluid_mesh");
CoSimIO_Info_SetString(export_settings, "connection_name", "test_connection");
CoSimIO_Info export_info = CoSimIO_ExportMesh(export_settings
        , number_of_nodes,number_of_elements,number_of_elements_connectivities
        , nodal_coordinates, elements_connectivities, elements_types);
```

The arguments are:

* `number_of_nodes`: Number of Nodes (== size(`nodal_coordinates`)/3)
* `number_of_elements`: Number of Elements (== size(`elements_types`))
* `number_of_elements_connectivities`: Number of Elements connectivities (== size(`elements_connectivities`))

* `nodal_coordinates`: A vector of doubles of 3D coordinates of each node in x1,y1,z1,x2,y2,z2,... format:
```c
double nodal_coordinates[] = {
    0.0, 2.5, 1.0, /*0*/
    2.0, 0.0, 1.5, /*1*/
    2.0, 2.5, 1.5, /*2*/
    4.0, 2.5, 1.7, /*3*/
    4.0, 0.0, 1.7, /*4*/
    6.0, 0.0, 1.8  /*5*/
};
```
* `elements_connectivities`: A vector of int containing the zero based index of each node in e1_1,e1_2,...,e2_1, e2_2,... format:
```c
int elements_connectivities[] = {
    0, 1, 2, /*1*/
    1, 3, 2, /*2*/
    1, 4, 3, /*3*/
    3, 4, 5, /*4*/
};
```

* `elements_types`: A vector of int containing the type of the elements. They are according to the vtk cell types, see [this link](https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf), page 9 & 10.
```c
int elements_types[] = {5,5,5,5}; // VTK_TRIANGLE
```

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```c
CoSimIO_Info import_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
CoSimIO_Info_SetString(import_settings, "connection_name", "test_connection");

CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings
    , &number_of_nodes,&number_of_elements,&number_of_elements_connectivities
    , &nodal_coordinates, &elements_connectivities, &elements_types);
```

This example can be found in [integration_tutorials/c/export_mesh.c](../../tests/integration_tutorials/c/export_mesh.c) and [integration_tutorials/c/import_mesh.c](../../tests/integration_tutorials/c/import_mesh.c).


## Tutorial 6: Kratos CoSimulation Library Overview
The overview of the Kratos CoSimulation Library can be found [here](../README.md#kratos-cosimulation-library-overview).

## Tutorial 7: Building Kratos with CoSimulation
The building instructions for the Kratos CoSimulation Library can be found [here](../README.md#building-kratos-with-cosimulation).

## Tutorial 8: Connecting/Disconnecting to/from Kratos
For connecting to Kratos it is very important to have in mind that Kratos also uses *CoSimIO* for interprocess communication so its python interface reflects the CoSimIO. So we may create a python script for connecting and disconnecting in the same way described in the [python tutorial](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tutorial/python/README.md):

```Python
from KratosMultiphysics.CoSimulationApplication import CoSimIO

connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "c_d_test")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", "c_d_test")

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")
```

Please note that the only change here is the import statement which loads the CoSimIO module which comes inside the KratosMultiphysics. You may find this python file in [Kratos/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/connect_disconnect.py](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/connect_disconnect.py)

Then you may run your executable with python script of Kratos from your working directory:

```shell
path/to/bin/tests_c/connect_disconnect_c_test & python3 path/to/connect_disconnect.py
```

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

From solver side first we recall the export data code described in tutorial 4 adjusting the connection name and data tag to the ones in our example python given above.

```c
    int data_size = 4;
    double data_to_send[] = {3, .1, .14, 3.14};

    // Creatint the export_settings
    CoSimIO_Info export_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(export_settings, "identifier", "data_exchange_1");
    CoSimIO_Info_SetString(export_settings, "connection_name", "im_exp_data");

    // Exporting the data
    CoSimIO_Info export_info = CoSimIO_ExportData(export_settings, data_size, data_to_send);
    // Freeing the export_info and export_settings
    CoSimIO_FreeInfo(export_info);
    CoSimIO_FreeInfo(export_settings);


    // After conneting we may import the data
    double* data;
    int data_allocated_size = 0;

    // Creating the import_settings
    CoSimIO_Info import_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(import_settings, "identifier", "data_exchange_2");
    CoSimIO_Info_SetString(import_settings, "connection_name", "im_exp_data");

    // Importing the data
    CoSimIO_Info import_info = CoSimIO_ImportData(import_settings, &data_allocated_size, &data);
    // Freeing the import_info and import_settings
    CoSimIO_FreeInfo(import_info);
    CoSimIO_FreeInfo(import_settings);
```
You may find this python file in [Kratos/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_data.py](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_data.py)

Please note that we should adjust the conncetion name and data tag in both sides to be the same.

Now for running the test:

```shell
path/to/bin/tests_c/export_import_data_c_test & python3 path/to/import_export_data.py
```

## Tutorial 10: Mesh Exchange with Kratos
In this step we send a mesh to Kratos and receive it back and we will check if they are the same. (like previous tutorial with data).

Recalling from what we had in tutorial 5 we just merge the export mesh and import mesh codes into one as we did for data exchage in previous tutorial:

```c
// Creating the export_settings
CoSimIO_Info export_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_settings, "identifier", "mesh_exchange_1");
CoSimIO_Info_SetString(export_settings, "connection_name", "im_exp_mesh");

// Exporting the mesh
CoSimIO_Info export_info = CoSimIO_ExportMesh(export_settings
    , export_number_of_nodes,export_number_of_elements,export_number_of_elements_connectivities
    , export_nodal_coordinates, export_elements_connectivities, export_elements_types);

// Freeing the export_info and export_settings
CoSimIO_FreeInfo(export_info);
CoSimIO_FreeInfo(export_settings);

// Now we import back the mesh
double* import_nodal_coordinates;
int import_number_of_nodes = 0;
int* import_elements_connectivities;
int import_number_of_elements_connectivities = 0;
int* import_elements_types;
int import_number_of_elements = 0;

// Creating the import_settings
CoSimIO_Info import_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "mesh_exchange_2");
CoSimIO_Info_SetString(import_settings, "connection_name", "im_exp_mesh");

// Importing the mesh
CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings
    , &import_number_of_nodes,&import_number_of_elements,&import_number_of_elements_connectivities
    , &import_nodal_coordinates, &import_elements_connectivities, &import_elements_types);

// Freeing the import_info and import_settings
CoSimIO_FreeInfo(import_info);
CoSimIO_FreeInfo(import_settings);

```
Now lets create the python script for Kratos which gets the mesh and returns it back to us. Here again the python script is very similar to one of the standard CoSimIO. Except for the ImportMesh and ExportMesh methods which are adapted to work directly with Kratos data structure. This means that the work with [ModelPart](https://github.com/KratosMultiphysics/Kratos/wiki/ModelPart-and-SubModelPart) instead of raw coordinates and connectivities arrays:

```Python
CoSimIO.ImportMesh(import_info, model_part)
CoSimIO.ExportMesh(export_info, model_part)
```

The ModelPart is the object containing mesh and its associated data. For creating the ModelPart we should first create a Model and then a ModelPart for imported mesh:

```Python
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO

model = KM.Model()
model_part = model.CreateModelPart("mp_test")
```

Then we should proceed to fill the settings and import the mesh:

```Python
import_info = CoSimIO.Info()
import_info.SetString("connection_name", "im_exp_mesh")
import_info.SetString("identifier", "mesh_exchange_1")
CoSimIO.ImportMesh(import_info, model_part)
```

We may check the imported ModelPart by printing it:

```Python
print(model_part)
```
Same for the export mesh:

```Python
export_info = CoSimIO.Info()
export_info.SetString("connection_name", "im_exp_mesh")
export_info.SetString("identifier", "mesh_exchange_2")
CoSimIO.ExportMesh(export_info, model_part)
```

Now putting everything together we have:


```Python
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO

model = KM.Model()
model_part = model.CreateModelPart("mp_test")

connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "im_exp_mesh")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

import_info = CoSimIO.Info()
import_info.SetString("connection_name", "im_exp_mesh")
import_info.SetString("identifier", "mesh_exchange_1")
CoSimIO.ImportMesh(import_info, model_part)

print(model_part)

export_info = CoSimIO.Info()
export_info.SetString("connection_name", "im_exp_mesh")
export_info.SetString("identifier", "mesh_exchange_2")
CoSimIO.ExportMesh(export_info, model_part)

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", "im_exp_mesh")

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")

```

You may find this python file in [Kratos/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_mesh.py](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_mesh.py)

Now for running the test:

```shell
path/to/bin/tests_c/export_import_mesh_c_test & python3 path/to/import_export_mesh.py
```

## Tutorial 11: Mapping with Kratos
This tutorial shows how to map data between (non matching) meshes with Kratos. It is based on tutorials 9 & 10.

In this tutorial we first send two meshes based on the same geometry but with different discretizations to Kratos. Those meshes are used as basis for the mapping. In Kratos teminology those are the origin and the destination.

After sending the meshes, data that is to be mapped is sent to Kratos. It is mapped and then sent back.

Note that the following tutorial contains only a subset of the mapping capabilities of Kratos. Check [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication) for a more detailed explanation.

The first step is to send the origin and destination meshes for mapping between:

```c
// Creating the export mesh settings
CoSimIO_Info export_mesh_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_origin");
CoSimIO_Info_SetString(export_mesh_settings, "connection_name", "mesh_mapping");

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
On the Kratos side we should receive these meshes as modelparts similar to what we did in the previous tutorial. Please note that you should create the origin and destination model parts before and then use the `ImportMesh` method to get the mesh:

```Python
# importing the Kratos library
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO

# create the Kratos ModelParts that contain the mesh
model = KM.Model()
model_part_origin = model.CreateModelPart("mp_origin")
model_part_destination = model.CreateModelPart("mp_destination")

# connect to CoSimIO
connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "mesh_mapping")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

# import meshes
import_mesh_info_o = CoSimIO.Info()
import_mesh_info_o.SetString("connection_name", "mesh_mapping")
import_mesh_info_o.SetString("identifier", "mesh_origin")
CoSimIO.ImportMesh(import_mesh_info_o, model_part_origin)

import_mesh_info_d = CoSimIO.Info()
import_mesh_info_d.SetString("connection_name", "mesh_mapping")
import_mesh_info_d.SetString("identifier", "mesh_destination")
CoSimIO.ImportMesh(import_mesh_info_d, model_part_destination)

```
After sending the meshes now we should send the data field in the origin (to be mapped afterward). From application side we will use the `CoSimIO_ExportData` function:

```c
// Creating the export origin data settings
CoSimIO_Info export_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_data_settings, "identifier", "data_to_map");
CoSimIO_Info_SetString(export_data_settings, "connection_name", "mesh_mapping");

// Exporting the origin data
CoSimIO_Info export_info = CoSimIO_ExportData(export_data_settings, export_data_size, export_data);

// Free memory
CoSimIO_FreeInfo(export_data_settings);
CoSimIO_FreeInfo(export_info);
```

On Kratos side, we should take this data and store it as nodal data in the imported origin model part. For doing that, first we should add those nodal data to the model part using the `AddNodalSolutionStepVariable` method. For example let's consider that we are recieving the temperature and we want to map it into the ambient temperature:

```Python
# allocate memory
model_part_origin.AddNodalSolutionStepVariable(KM.TEMPERATURE)
model_part_destination.AddNodalSolutionStepVariable(KM.AMBIENT_TEMPERATURE)

model_part_origin.AddNodalSolutionStepVariable(KM.VELOCITY)
model_part_destination.AddNodalSolutionStepVariable(KM.MESH_VELOCITY)
```

Please note that above methods should be called before importing the mesh. For the list of the variables which are defined in the core of Kratos and their types, please check the [variables.h](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/variables.h)

After preparing the model parts we can import the data from the application and store it in the origin model part using the `ImportData` method. For example for storing the data as temperature in origin model part we will have:

```Python
# import data to be mapped
import_data_info = CoSimIO.Info()
import_data_info.SetString("connection_name", "mesh_mapping")
import_data_info.SetString("identifier", "data_to_map")
CoSimIO.ImportData(import_data_info, model_part_origin, KM.TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Please note that here the `CoSimIO.DataLocation.NodeHistorical` argument is saying that we want to store the imported data as historical value in the nodes of this model part. So the size of the data should be coherent with the number of the nodes in the model parts.

Now is time to do the mapping. The first step would be to import the mapping application which provides the mapping mechanism:

```Python
import KratosMultiphysics.MappingApplication as KratosMapping
```

Then we can create a mapper using the mapper factory:

```Python

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

```Python
# map scalar quantities
mapper.Map(KM.TEMPERATURE, KM.AMBIENT_TEMPERATURE)

# map vector quantities
mapper.Map(KM.VELOCITY, KM.MESH_VELOCITY)
```
And finally to send back the mapped data to the application from kratos side we will use the `ExportData` method:

```Python
# export mapped data
export_data_info = CoSimIO.Info()
export_data_info.SetString("connection_name", "mesh_mapping")
export_data_info.SetString("identifier", "mapped_data")
CoSimIO.ExportData(export_data_info, model_part_destination, KM.AMBIENT_TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)
```
Again here the `CoSimIO.DataLocation.NodeHistorical` arguments denotes that we take the `AMBIENT_TEMPERATURE` from each node historical data and export it.

In application side we should import the data as described in data transfer tutorial:

```c
// Import the mapped destination data
double* mapped_data;
int data_allocated_size = 0;

// Creating the import data settings
CoSimIO_Info import_mapped_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_mapped_data_settings, "identifier", "mapped_data");
CoSimIO_Info_SetString(import_mapped_data_settings, "connection_name", "mesh_mapping");

// Importing the data
CoSimIO_Info import_info = CoSimIO_ImportData(import_mapped_data_settings, &data_allocated_size, &mapped_data);

// Free memory
CoSimIO_FreeInfo(import_info);
CoSimIO_FreeInfo(import_mapped_data_settings);
```

Now putting everything together in the application side we have:
```c
// Creating the export mesh settings
CoSimIO_Info export_mesh_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_mesh_settings, "identifier", "mesh_origin");
CoSimIO_Info_SetString(export_mesh_settings, "connection_name", "mesh_mapping");

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

// Creating the export origin data settings
CoSimIO_Info export_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_data_settings, "identifier", "data_to_map");
CoSimIO_Info_SetString(export_data_settings, "connection_name", "mesh_mapping");

// Exporting the origin data
CoSimIO_Info export_info = CoSimIO_ExportData(export_data_settings, export_data_size, export_data);

// Free memory
CoSimIO_FreeInfo(export_data_settings);
CoSimIO_FreeInfo(export_info);

// Import the mapped destination data
double* mapped_data;
int data_allocated_size = 0;

// Creating the import data settings
CoSimIO_Info import_mapped_data_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_mapped_data_settings, "identifier", "mapped_data");
CoSimIO_Info_SetString(import_mapped_data_settings, "connection_name", "mesh_mapping");

// Importing the data
CoSimIO_Info import_info = CoSimIO_ImportData(import_mapped_data_settings, &data_allocated_size, &mapped_data);

// Free memory
CoSimIO_FreeInfo(import_info);
CoSimIO_FreeInfo(import_mapped_data_settings);
```

And in the Kratos side the python script would be:

```Python
# importing the Kratos library
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO
import KratosMultiphysics.MappingApplication as KratosMapping

# create the Kratos ModelParts that contain the mesh
model = KM.Model()
model_part_origin = model.CreateModelPart("mp_origin")
model_part_destination = model.CreateModelPart("mp_destination")

# allocate memory
model_part_origin.AddNodalSolutionStepVariable(KM.TEMPERATURE)
model_part_destination.AddNodalSolutionStepVariable(KM.AMBIENT_TEMPERATURE)

model_part_origin.AddNodalSolutionStepVariable(KM.VELOCITY)
model_part_destination.AddNodalSolutionStepVariable(KM.MESH_VELOCITY)

# connect to CoSimIO
connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "mesh_mapping")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

# import meshes
import_mesh_info_o = CoSimIO.Info()
import_mesh_info_o.SetString("connection_name", "mesh_mapping")
import_mesh_info_o.SetString("identifier", "mesh_origin")
CoSimIO.ImportMesh(import_mesh_info_o, model_part_origin)

import_mesh_info_d = CoSimIO.Info()
import_mesh_info_d.SetString("connection_name", "mesh_mapping")
import_mesh_info_d.SetString("identifier", "mesh_destination")
CoSimIO.ImportMesh(import_mesh_info_d, model_part_destination)

print(model_part_origin)
print(model_part_destination)

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

# import data to be mapped
import_data_info = CoSimIO.Info()
import_data_info.SetString("connection_name", "mesh_mapping")
import_data_info.SetString("identifier", "data_to_map")
CoSimIO.ImportData(import_data_info, model_part_origin, KM.TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)

# map scalar quantities
mapper.Map(KM.TEMPERATURE, KM.AMBIENT_TEMPERATURE)

# map vector quantities
mapper.Map(KM.VELOCITY, KM.MESH_VELOCITY)

# export mapped data
export_data_info = CoSimIO.Info()
export_data_info.SetString("connection_name", "mesh_mapping")
export_data_info.SetString("identifier", "mapped_data")
CoSimIO.ExportData(export_data_info, model_part_destination, KM.AMBIENT_TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)

# disconnect from CoSimIO
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", "mesh_mapping")

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")
```