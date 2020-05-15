# Tutorial for integrating the _CoSimIO_ using the C interface

This tutorial helps you through to integrate the _CoSimIO_ into a solver using the C interface.

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler because the library by itself is in CPP and only a small C wrapper provides its C interface. most of the major CPP compilers in Windows, Linux, and Mac are C++11 compatible. You may find a detailed list of C++11 compatible compilers in https://en.cppreference.com/w/cpp/compiler_support#cpp11 


## Tutorial 1: Building
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

You may find this example in hello.c file in the `solver_integration/c` folder


## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `CoSimIO_Connect()` function:
```c
// The connect must be called before any CosimIO function called
CoSimIO_Info connect_info = CoSimIO_Connect(settings);
```

First of all, you may notice that `Connect()` function takes an `Info` as its arguments. So first you should create it using `CoSimIO_CreateInfo()` function:

```c
CoSimIO_Info settings=CoSimIO_CreateInfo();
```

It is important to mention that the `CoSimIO_Info` is a pointer to the cpp info class which is allocated by the `CoSimIO_CreateInfo()`. So it is don't forget to free it when is not needed anymore using `CoSimIO_FreeInfo()` function. This container can be used to pass additional information about the solver or connection settings to the CoSimIO:

```c
CoSimIO_Info_SetString(settings, "connection_name", "test_connection"); // The connection name must be unique for each connection between two solvers
CoSimIO_Info_SetString(settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
CoSimIO_Info_SetInt(settings, "echo_level", 1);
CoSimIO_Info_SetString(settings, "solver_version", "1.25");
```
This function returns a `Info` object containing information about the connection which can be queried using `CoSimIO_Info_GetInt` function:

```c
CoSimIO_Info_GetInt(info, "connection_status");
```

Now putting together everything:

```c
// CoSimulation includes
#include "c/co_sim_io_c.h"

int main()
{
    CoSimIO_Info settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "connection_name", "test_connection"); // The connection name must be unique for each connection between two solvers
    CoSimIO_Info_SetString(settings, "solver_name", "my_solver"); // Not to be confused with the connection name.
    CoSimIO_Info_SetInt(settings, "echo_level", 1);
    CoSimIO_Info_SetString(settings, "solver_version", "1.25");

    // The connect must be called before any CosimIO function called
    CoSimIO_Info connect_info = CoSimIO_Connect(settings);

    if(CoSimIO_Info_GetInt(connect_info, "connection_status") != CoSimIO_Connected)
        return 1;
    
    // Don't forget to release the connect_info after getting your information
    CoSimIO_FreeInfo(connect_info);

    // Now you may call any CoSimIO functions 
    // ...

    // Here you may use the info but cannot call any CoSimIO function anymore
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(settings); // disconnect afterwards
    if(CoSimIO_Info_GetInt(disconnect_info, "connection_status") != CoSimIO_Disconnected)
        return 1;

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
```

You may find this example in `connect_disconect.cpp` file in the `solver_integration/c` folder

## Tutorial 4: Data Exchange
One of the important missions of the CoSimIO is to send and recieve data between processes. The `CoSimIO_ExportData()` function can be used to send data to the Kratos or directly to another solver. For exporting the following array 4 doubles:

```c
double data_to_send[] = {3.14, 3.14, 3.14, 3.14};
```
First we should create a setting which provides an identifier (like "velocity_of_structure") and the connection name:

```c
// Creatint the export_settings 
CoSimIO_Info export_settings=CoSimIO_CreateInfo();
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
CoSimIO_Info import_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "vector_of_pi");
CoSimIO_Info_SetString(import_settings, "connection_name", "test_connection");
```

In this case we just pass an empty pointer and specifying to the `ImportData()` that should allocate the data by itself. So, in order to ensure the memory coherance, the `CoSimIO_Free()` function should be used instead of standard `free()` function:

```c
// Freeing the data using CoSimIO_Free. (Not the standard free())
CoSimIO_Free(data);
```
You may also allocate the memory for data by `CoSimIO_Malloc()` function. If the allocated size is larger that imported data then there is no reallocation is done but if imported data is larger, then only if the data is allocated by `CoSimIO_Malloc()` function a reallocation will be done. If not, it will gives an error.


## Tutorial 5: Mesh Exchange
After seeing how we transfer raw data between solvers, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```c++
CoSimIO::Info info;
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", "test_connection");
info = CoSimIO::ExportMesh(info,nodal_coordinates, elements_connectivities, elements_types);
```

The arguments are:

* `nodal_coordinates`: A vector of doubles of 3D coordinates of each node in x1,y1,z1,x2,y2,z2,... format:
```c++
std::vector<double> nodal_coordinates{
    0.0, 2.5, 1.0, /*0*/
    2.0, 0.0, 1.5, /*1*/
    2.0, 2.5, 1.5, /*2*/
    4.0, 2.5, 1.7, /*3*/
    4.0, 0.0, 1.7, /*4*/
    6.0, 0.0, 1.8  /*5*/
    };
```
* `elements_connectivities`: A vector of int containing the zero based index of each node in e1_1,e1_2,...,e2_1, e2_2,... format:
```c++
std::vector<int> elements_connectivities = {
    0, 1, 2, /*1*/
    1, 3, 2, /*2*/
    1, 4, 3, /*3*/
    3, 4, 5, /*4*/
};
```

* `elements_types`: A vector of int containing the type of the elements. They are according to the vtk cell types, see [this link](https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf), page 9 & 10.
```c++
std::vector<int> elements_types = {5,5,5,5}; // VTK_TRIANGLE
```


