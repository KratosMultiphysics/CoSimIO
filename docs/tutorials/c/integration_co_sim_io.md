# Tutorial for integrating the _CoSimIO_ using C interface

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [What you need](#what-you-need)
- [Building the CoSimIO](#building-the-cosimio)
- [Hello CosimIO](#hello-cosimio)
- [Connecting and Disconnecting](#connecting-and-disconnecting)
- [Data Exchange](#data-exchange)
- [Mesh Exchange](#mesh-exchange)
- [Next steps](#next-steps)

<!-- /code_chunk_output -->
---

This tutorial helps you to integrate the _CoSimIO_ into a solver/software-tool using the C++ interface.

## What you need
- Downloading the _CosimIO_ from the repository:
  ```bash
  git clone https://github.com/KratosMultiphysics/CoSimIO.git
  ```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)

- [CMake](https://cmake.org/)

## Building the CoSimIO
The C version of CosimIO is implemented in the [co_sim_io_c.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/c/co_sim_io_c.cpp). It can be used after compiling CoSimIO into a shared library and linking against it. One can use [build_c.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.sh) for compiling it. Check [here](../../build_options.md) for the available build options.

```bash
$ bash scripts/build_c.sh
```

The shared library will be installed in the `bin/` folder. After building and linking it to your project, you may use the interface defined in `co_sim_io_c.h`:

```c
// CoSimulation includes
#include "c/co_sim_io_c.h"

int main(){
    return 0;
}
```

Please don't forget to add the folder `co_sim_io` to your include path so that the compiler can find the includes
Example:
```bash
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/path/to/CoSimIO/co_sim_io
```

## Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```c
CoSimIO_Info hello_info = CoSimIO_Hello();
```

Please note that this function like other functions in _CoSimIO_ returns an `Info` object. This object is a versatile container holding important information about the operation that has been done. See [here](../../info/info_c.md) for the documentation. In this case, it contains the version of the _CoSimIO_ library which can be printed using `CoSimIO_PrintInfo()` function:

```c
CoSimIO_PrintInfo(stdout, hello_info);
```

The `Info` class also provides `Get...` methods for retrieving certain values knowing their type:

```c
int major_version = CoSimIO_Info_GetInt(hello_info, "major_version");
int minor_version = CoSimIO_Info_GetInt(hello_info, "minor_version");
const char* patch_version = CoSimIO_Info_GetString(hello_info, "patch_version");
```

Finally it is **very important** to free the info using the `CoSimIO_FreeInfo()` function:

```c
CoSimIO_FreeInfo(hello_info);
```

This example can be found in [integration_tutorials/c/hello.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/hello.c).


## Connecting and Disconnecting
The first step to establish a connection to Kratos CoSimulation is to use the `CoSimIO_Connect` method:
```c
// The connect must be called before any CosimIO method
CoSimIO_Info connect_info = CoSimIO_Connect(settings);
```

First of all, you may notice that `Connect()` function takes an `Info` as its arguments. So first you should create it using `CoSimIO_CreateInfo()` function:

```c
CoSimIO_Info settings = CoSimIO_CreateInfo();
```

It is important to mention that the `CoSimIO_Info` is a pointer to the cpp info class which is allocated by the `CoSimIO_CreateInfo()`. So it is don't forget to free it when is not needed anymore using `CoSimIO_FreeInfo()` function. This container can be used to pass additional information about the solver/software-tool or connection settings to the CoSimIO:

```c
CoSimIO_Info_SetString(settings, "my_name", "the_name_of_this_solver"); // my name
CoSimIO_Info_SetString(settings, "connect_to", "the_other_solver_name"); // to whom I want to connect toto
CoSimIO_Info_SetInt(settings, "echo_level", 1);
CoSimIO_Info_SetString(settings, "version", "1.25");
```

It is very important that the settings for `my_name` and `connect_to` are set correctly, otherwise the connection cannot be established!

E.g in the above example one code sets `my_name` as `cpp_connect_disconnect_a` and `connect_to` as `cpp_connect_disconnect_b`. This means that the other code has to use the same settings but reversed: `my_name` must be `cpp_connect_disconnect_b` and `connect_to` must be `cpp_connect_disconnect_a`.

This method returns a `Info` object containing information about the connection which can be queried using `CoSimIO_Info_Get...` method. For further calls to `CoSimIO` it is necessary to get the `connection_name`:
```c
CoSimIO_Info_GetString(info, "connection_name");
CoSimIO_Info_GetInt(info, "connection_status");
```

Similar things work for the `Disconnect` method.
Now putting together everything:

```c
// CoSimulation includes
#include "c/co_sim_io_c.h"

int main()
{
    CoSimIO_Info settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "my_name", "c_connect_disconnect_a");
    CoSimIO_Info_SetString(settings, "connect_to", "c_connect_disconnect_b");
    CoSimIO_Info_SetInt(settings, "echo_level", 1);
    CoSimIO_Info_SetString(settings, "version", "1.25");

    // The connect must be called before any CosimIO function called
    CoSimIO_Info connect_info = CoSimIO_Connect(settings);
    CoSimIO_FreeInfo(settings);

    // The connect_info contains now:
    // - The name of the connection ("connection_name") to be used for further calls to CoSimIO
    // - The status of the connection ("connection_status")

    const char* connection_name = CoSimIO_Info_GetString(connect_info, "connection_name"); // getting name of connection for future calls

    if (CoSimIO_Info_GetInt(connect_info, "connection_status") != CoSimIO_Connected)
        return 1;

    // Now you may call any CoSimIO functions
    // ...

    // Here you may use the info but cannot call any CoSimIO function anymore
    CoSimIO_Info disconnect_settings=CoSimIO_CreateInfo();
    CoSimIO_Info_SetString(settings, "connection_name", connection_name); // connection_name is obtained from calling "Connect"
    CoSimIO_Info disconnect_info = CoSimIO_Disconnect(disconnect_settings); // disconnect afterwards
    if(CoSimIO_Info_GetInt(disconnect_info, "connection_status") != CoSimIO_Disconnected)
        return 1;

    // Don't forget to release the settings and info
    CoSimIO_FreeInfo(connect_info);
    CoSimIO_FreeInfo(disconnect_settings);
    CoSimIO_FreeInfo(disconnect_info);

    return 0;
}
```

This example can be found in [integration_tutorials/c/connect_disconnect_a.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/connect_disconnect_a.c) and [integration_tutorials/c/connect_disconnect_b.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/connect_disconnect_b.c).


## Data Exchange
Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Data Exchange.

One of the important missions of the CoSimIO is to send and recieve data between different solvers/software tools. The `CoSimIO_ExportData` method can be used to send data to the Kratos or directly to another solver/software-tool. For exporting the following array 4 doubles:

```c
double data_to_send[] = {3.14, 3.14, 3.14, 3.14};
```

```c
// Creating the export_settings
CoSimIO_Info export_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_settings, "identifier", "vector_of_pi");
CoSimIO_Info_SetString(export_settings, "connection_name", connection_name); // connection_name is obtained from calling "Connect"
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

// Creating the import_settings
CoSimIO_Info import_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "vector_of_pi");
CoSimIO_Info_SetString(import_settings, "connection_name", connection_name); // connection_name is obtained from calling "Connect"
```

In this case we just pass an empty pointer and specifying to the `ImportData()` that should allocate the data by itself. So, in order to ensure the memory coherance, the `CoSimIO_Free()` function should be used instead of standard `free()` function:

```c
// Freeing the data using CoSimIO_Free. (Not the standard free())
CoSimIO_Free(data);
```
You may also allocate the memory for data by `CoSimIO_Malloc()` function. If the allocated size is larger that imported data then there is no reallocation is done but if imported data is larger, then only if the data is allocated by `CoSimIO_Malloc()` function a reallocation will be done. If not, it will gives an error.

This example can be found in [integration_tutorials/c/export_data.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/export_data.c) and [integration_tutorials/c/import_data.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/import_data.c).


## Mesh Exchange
Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Mesh Exchange.

After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```c
CoSimIO_Info export_settings = CoSimIO_CreateInfo();
CoSimIO_Info_SetString(export_settings, "identifier", "fluid_mesh");
CoSimIO_Info_SetString(export_settings, "connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("name_of_model_part_to_export");

CoSimIO_Info export_info = CoSimIO_ExportMesh(export_settings, model_part);
```

The argument `model_part` is of type `CoSimIO_ModelPart`. Its usage is explained [here](../../model_part/model_part_c.md).

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```c
CoSimIO_Info import_settings=CoSimIO_CreateInfo();
CoSimIO_Info_SetString(import_settings, "identifier", "fluid_mesh");
CoSimIO_Info_SetString(import_settings, "connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("name_of_imported_model_part");

CoSimIO_Info import_info = CoSimIO_ImportMesh(import_settings, model_part);
```

This example can be found in [integration_tutorials/c/export_mesh.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/export_mesh.c) and [integration_tutorials/c/import_mesh.c](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/c/import_mesh.c).

## Next steps
In the [next tutorial](basic_data_exchange_with_kratos.md), a connection to Kratos is established and basic data exchange with Kratos is done.
