# Tutorial for integrating the _CoSimIO_ using C++ interface

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
The C++ version of the CoSimIO is implemented in [co_sim_io.hpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/co_sim_io.hpp). It can be used after compiling CoSimIO into a shared library and linking against it. Check [here](../../build_options.md) for the available build options.

Several options for using/integrating the _CoSimIO_ exist:
- Using CMake:
    - Integrating _CoSimIO_ as a subproject into the host with CMake:
        Here the _CoSimIO_ is directly integrated into the host code and added with `add_subdirectory(CoSimIO)`. CMake then takes care of compiling and installing the _CoSimIO_.
        Kratos uses this way to integrate the CoSimIO, check the [`CMakeLists.txt` of the CoSimulationApplication for details](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/CMakeLists.txt)
    - Compiling _CoSimIO_ outside the host project:
        One can use [build_cpp.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.sh) or [build_cpp.bat](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.bat) for compiling it.

        - For *GNU/Linux* or *MacOS*:
            ```bash
            $ bash scripts/build_cpp.sh
            ```

        - For Windows OS:

            ```cmd
            $ scripts/build_cpp.bat
            ```

    After compiling, the headers need to be included (e.g. with `include_directories`) and the `co_sim_io` shared library needs to be linked (e.g with `target_link_libraries`).
    ```
    include_directories(path/to/CoSimIO/co_sim_io)
    target_link_libraries(my_executable co_sim_io)
    ```

- Compiling _CoSimIO_ outside the host and manually integrating it:
    One can use [build_cpp.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.sh) or [build_cpp.bat](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.bat) for compiling it.

    - For *GNU/Linux* or *MacOS*:
        ```bash
        $ bash scripts/build_cpp.sh
        ```

    - For Windows OS:

        ```cmd
        $ scripts/build_cpp.bat
        ```

    Is important that the compiler can find the header files of the _CoSimIO_. For this, one can add the folder `co_sim_io` to the include path.
    Example:
    ```bash
    export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/path/to/CoSimIO/co_sim_io
    ```

    Furthermore it is required to link against the `co_sim_io` shared library.

The shared library `co_sim_io` will be installed in the `bin/` folder. After building _CoSimIO_, is necessary to add the path where the shared library `co_sim_io` was installed to (`bin/`) to the library path of the system such that the it can be linked against. Depending on the operating system this can be e.g. `PATH` or `LD_LIBRARY_PATH`. This also needs to be available at runtime, otherwise errors when strating the execution will occur. In linux this can be achieved with the following:

```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH$:$HOME/path/to/CoSimIO/bin"
```

After building and linking it to your project, you may use the interface defined in `co_sim_io.hpp`:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"

int main()
{
    return 0;
}
```

## Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```c++
CoSimIO::Info info = CoSimIO::Hello();
```

Please note that this method like other methods in _CoSimIO_ returns a `CoSimIO::Info` object. This object is a versatile container holding important information about the operation that has been done. See [here](../../info/info_cpp.md) for the documentation. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```c++
std::cout << info << std::endl;
```

The `Info` class also provides the `Get` method for retrieving certain values knowing their type:

```c++
int major_version = info.Get<int>("major_version");
int minor_version = info.Get<int>("minor_version");
std::string patch_version = info.Get<std::string>("patch_version");
```

This example can be found in [integration_tutorials/cpp/hello.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/hello.cpp).


## Connecting and Disconnecting
The first step to establish a connection to Kratos CoSimulation is to use the `Connect` method:
```c++
// The connect must be called before any CosimIO method
auto info = CoSimIO::Connect(settings);
```

First of all, you may notice that `Connect` method takes a `CoSimIO::Info` as its argument. This container can be used to pass additional information about the solver/software-tool or connection settings to the _CoSimIO_:

```c++
CoSimIO::Info settings;
settings.Set("my_name", "cpp_connect_disconnect_a");    // my name
settings.Set("connect_to", "cpp_connect_disconnect_b"); // to whom I want to connect to
settings.Set("echo_level", 1);
settings.Set("version", "1.25");
```

It is very important that the settings for `my_name` and `connect_to` are set correctly, otherwise the connection cannot be established!

E.g in the above example one code sets `my_name` as `cpp_connect_disconnect_a` and `connect_to` as `cpp_connect_disconnect_b`. This means that the other code has to use the same settings but reversed: `my_name` must be `cpp_connect_disconnect_b` and `connect_to` must be `cpp_connect_disconnect_a`.

This method returns a `Info` object containing information about the connection which can be queried using `Get` method. For further calls to `CoSimIO` it is necessary to get the `connection_name`:

```c++
std::string connection_name = info.Get<std::string>("connection_name");
```
Similar things work for the `Disconnect` method.
Now putting together everything:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"

int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_connect_disconnect_a");    // my name
    settings.Set("connect_to", "cpp_connect_disconnect_b"); // to whom I want to connect to
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    std::string connection_name = info.Get<std::string>("connection_name"); // getting name of connection for future calls
    if (info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Connected)
        return 1;
    // Now you may call any CoSimIO methods like ImportData, ExportData, etc.

    // ...
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    // Here you may use the info but cannot call any CoSimIO method anymore
    if (info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Disconnected)
        return 1;

    return 0;
}
```

This example can be found in [integration_tutorials/cpp/connect_disconnect_a.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/connect_disconnect_a.cpp) and [integration_tutorials/cpp/connect_disconnect_b.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/connect_disconnect_b.cpp).


## Data Exchange
Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Data Exchange.

One of the important missions of the CoSimIO is to send and recieve data between different solvers/software tools. The `ExportData` method can be used to send data to the Kratos or directly to another solver/software-tool:

```c++
std::vector<double> data_to_send(4, 3.14);
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"
info = CoSimIO::ExportData(info, data_to_send);
```
The `ImportData` should be used on the other side to recieve data:

```c++
std::vector<double> receive_data;
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"
info = CoSimIO::ImportData(info, receive_data);
```

It is important to mention that `ImportData` will clear and resize the vector if needed.

This example can be found in [integration_tutorials/cpp/export_data.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/export_data.cpp) and [integration_tutorials/cpp/import_data.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/import_data.cpp).


## Mesh Exchange
Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Mesh Exchange.

After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh` method:


```c++
CoSimIO::Info info;
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO::ModelPart model_part("name_of_model_part_to_export");

export_info = CoSimIO::ExportMesh(info, model_part);
```

The argument `model_part` is of type `CoSimIO::ModelPart` and contains a mesh. Its usage is explained [here](../../model_part/model_part_cpp.md).

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```c++
info.Clear();
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", connection_name); // connection_name is obtained from calling "Connect"

CoSimIO::ModelPart model_part("name_of_imported_model_part");

import_info = CoSimIO::ImportMesh(info, model_part);
```

This example can be found in [integration_tutorials/cpp/export_mesh.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/export_mesh.cpp) and [integration_tutorials/cpp/import_mesh.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/import_mesh.cpp).

## Next steps
In the [next tutorial](basic_data_exchange_with_kratos.md), a connection to Kratos is established and basic data exchange with Kratos is done.
