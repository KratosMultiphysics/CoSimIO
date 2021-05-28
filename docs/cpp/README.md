# Tutorial for integrating the _CoSimIO_ using C++ interface

This tutorial helps you to integrate the _CoSimIO_ into a solver/software-tool using the C++ interface.

## Overview

- [What you need](#what-you-need)
- [Tutorial 1: Building the CoSimIO](#tutorial-1-building-the-cosimio)
- [Tutorial 2: Hello CoSimIO](#tutorial-2-hello-cosimio)
- [Tutorial 3: Connecting and Disconnecting](#tutorial-3-connecting-and-disconnecting)
- [Tutorial 4: Data Exchange](#tutorial-4-data-exchange)
- [Tutorial 5: Mesh Exchange](#tutorial-5-mesh-exchange)
- [Tutorial 6: Kratos CoSimulation Library Overview](#tutorial-6-kratos-cosimulation-library-overview)
- [Tutorial 7: Building Kratos with CoSimulation](#tutorial-7-building-kratos-with-cosimulation)



<!--
- [Tutorial 8: Connecting/Disconnecting to/from Kratos](#tutorial-8-Connecting/Disconnecting-tofrom-Kratos)
- [Tutorial 9: Data Exchange with Kratos](#tutorial-9-Data-Exchange-with-Kratos)
- [Tutorial 10: Mesh Exchange with Kratos](#tutorial-10-Mesh-Exchange-with-Kratos)
- [Tutorial 11: Mapping with Kratos](#tutorial-11-Mapping-with-Kratos)
-->

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers in https://en.cppreference.com/w/cpp/compiler_support#cpp11


## Tutorial 1: Building the CoSimIO
The C++ version of CosimIO is header only and for building you may only include the [co_sim_io.hpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/co_sim_io.hpp) in your project file and compile it with your code:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"

int main(){
    return 0;
}
```

Please don't forget to add the CoSimIO folder to your include path.


## Tutorial 2: Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```c++
auto info = CoSimIO::Hello();
```

Please note that this method like other methods in _CoSimIO_ returns an `Info` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```c++
std::cout << info << std::endl;
```

Info class also provides get method for retriving a certain value knowing its type:

```c++
int major_version = info.Get<int>("major_version");
int minor_version = info.Get<int>("minor_version");
std::string patch_version = info.Get<std::string>("patch_version");
```

This example can be found in [integration_tutorials/cpp/hello.cpp](../../tests/integration_tutorials/cpp/hello.cpp).


## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:
```c++
// The connect must be called before any CosimIO method called
auto info = CoSimIO::Connect(settings);
```

First of all, you may notice that `Connect()` method takes an `Info` as its arguments. This container can be used to pass additional information about the solver/software-tool or connection settings to the _CoSimIO_:

```c++
CoSimIO::Info settings;
settings.Set("my_name", "cpp_connect_disconnect_a"); // my name
settings.Set("connect_to", "cpp_connect_disconnect_b"); // to whom I want to connect to
settings.Set("echo_level", 1);
settings.Set("version", "1.25");
```
This method returns a `Info` object containing information about the connection which can be queried using Get method. For further calls to `CoSimIO` it is necessary to get the `connection_name`:

```c++
std::string connection_name = info.Get<std::string>("connection_name");
```

Now putting together everything:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"
int main(){
    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_connect_disconnect_a"); // my name
    settings.Set("connect_to", "cpp_connect_disconnect_b"); // to whom I want to connect to
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    std::string connection_name = info.Get<std::string>("connection_name"); // getting name of connection for future calls
    if(info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Connected)
        return 1;
    // Now you may call any CoSimIO methods like ImportData, ExportData, etc.

    // ...
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    // Here you may use the info but cannot call any CoSimIO method anymore
    if(info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Disconnected)
        return 1;

    return 0;
}
```

This example can be found in [integration_tutorials/cpp/connect_disconnect_a.cpp](../../tests/integration_tutorials/cpp/connect_disconnect_a.cpp) and [integration_tutorials/cpp/connect_disconnect_b.cpp](../../tests/integration_tutorials/cpp/connect_disconnect_b.cpp).


## Tutorial 4: Data Exchange
One of the important missions of the CoSimIO is to send and recieve data between processes. The `ExportData()` method can be used to send data to the Kratos or directly to another solver/software-tool:

```c++
std::vector<double> data_to_send(4,3.14);
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"
info = CoSimIO::ExportData(info, data_to_send);
```
The `ImportData()` should be used on the other side to recieve data:

```c++
std::vector<double> receive_data;
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"
info = CoSimIO::ImportData(info, receive_data);
```

It is important to mention that the `ImportData()` will clear and resize the vector if needed.

This example can be found in [integration_tutorials/cpp/export_data.cpp](../../tests/integration_tutorials/cpp/export_data.cpp) and [integration_tutorials/cpp/import_data.cpp](../../tests/integration_tutorials/cpp/import_data.cpp).


## Tutorial 5: Mesh Exchange
After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```c++
CoSimIO::Info info;
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO::ModelPart model_part("name_of_model_part_to_export");

export_info = CoSimIO::ExportMesh(info, model_part);
```

The argument `model_part` is of type `CoSimIO::ModelPart`. Its usage is explained [here](model_part.md).

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```c++
info.Clear();
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO::ModelPart model_part("name_of_imported_model_part");

import_info = CoSimIO::ImportMesh(info, model_part);
```

This example can be found in [integration_tutorials/cpp/export_mesh.cpp](../../tests/integration_tutorials/cpp/export_mesh.cpp) and [integration_tutorials/cpp/import_mesh.cpp](../../tests/integration_tutorials/cpp/import_mesh.cpp).


## Tutorial 6: Kratos CoSimulation Library Overview
The overview of the Kratos CoSimulation Library can be found [here](../README.md#kratos-cosimulation-library-overview).


## Tutorial 7: Building Kratos with CoSimulation
The building instructions for the Kratos CoSimulation Library can be found [here](../README.md#building-kratos-with-cosimulation).

<!--
## Tutorial 8: Connecting/Disconnecting to/from Kratos
coming soon!


## Tutorial 9: Data Exchange with Kratos
coming soon!


## Tutorial 10: Mesh Exchange with Kratos
coming soon!


## Tutorial 11: Mapping with Kratos
coming soon! -->
