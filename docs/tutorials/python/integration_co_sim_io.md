# Tutorial for integrating the _CoSimIO_ using the Python interface

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

This tutorial helps you to integrate the _CoSimIO_ into a solver/software-tool using the Python interface.

## What you need

- Downloading the _CosimIO_ from the repository:
  ```bash
  git clone https://github.com/KratosMultiphysics/CoSimIO.git
  ```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)

- [CMake](https://cmake.org/)

- A developer version of [Python](https://www.python.org/). It is recommended to use Python >= v3.5

## Building the CoSimIO
The Python interface is located in [co_sim_io/python](https://github.com/KratosMultiphysics/CoSimIO/tree/master/co_sim_io/python) folder of the repository. It uses the [pybind11 library](https://github.com/pybind/pybind11) for exposing the C++ code to Python. It is provided in the [expternal_libraries/pybind11](https://github.com/KratosMultiphysics/CoSimIO/tree/master/external_libraries/pybind11) folder of this repo.

One may use the [build_python.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_python.sh) or [build_python.bat](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_python.bat) script from the _CoSimIO_ root folder to create a library of the python interface:

- For *GNU/Linux* or *MacOS*:
    ```bash
    $ bash scripts/build_python.sh
    ```

- For Windows OS:

    ```cmd
    $ scripts/build_python.bat
    ```
This will compile the binaries and install them into the `bin/` subfolder of _CoSimIO_, together with the _CoSimIO_ Python module. Check [here](../../build_options.md) for the available build options.

The created folder structure should look like this:
```
| - bin
  | - PyCoSimIO.cpython-35m-x86_64-linux-gnu.so # (Linux version)
    - CoSimIO
    | - __init__.py
```

Remember to add the `/bin` folder to your `PYTHONPATH`. For Linux this should look like the following:
```bash
export PYTHONPATH=$PYTHONPATH:/path/to/CoSimIO/bin
```

Usually pybind automatically detects the Python installation. Sometimes it can however be necessary to explicitly specify the target Python version. This can be achieved through CMake by setting `PYBIND11_PYTHON_VERSION` or an exact Python installation can be specified with `PYTHON_EXECUTABLE`. For example:
```cmake
-DPYBIND11_PYTHON_VERSION=3.6
# or
-DPYTHON_EXECUTABLE=path/to/python_executable
```
see also the corresponding [pybind documentation](https://pybind11.readthedocs.io/en/stable/compiling.html#configuration-variables).

If Python cannot find the _CoSimIO_ then check if the version of Python used for the compilation is the same as the version of Python used. E.g. when compiling _CoSimIO_ with Python 3.6 it is not possible to import it when using Python 3.8. Here Python issues a `ModuleNotFoundError`.
This can easily be checked with the file extension of the compiled _CoSimIO_ module.
Example: When using Python 3.5 the name of the compiled module will be sth like `PyCoSimIO.cp35-win_amd64.pyd` (Windows), `PyCoSimIO.cpython-35m-x86_64-linux-gnu.so` (Linux) or `PyCoSimIO.cpython-35m-darwin.so` (MacOS).


## Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```py
import CoSimIO
info = CoSimIO.Hello()
```

Please note that this method like other methods in _CoSimIO_ returns a `CoSimIO::Info` object. This object is a versatile container holding important information about the operation that has been done. See [here](../../info/info_python.md) for the documentation. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```py
print(info)
```

The `Info` class also provides `Get...` methods for retrieving certain values knowing their type:

```py
major_version = info.GetInt("major_version")
minor_version = info.GetInt("minor_version")
patch_version = info.GetString("patch_version")
```

This example can be found in [integration_tutorials/python/hello.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/hello.py).

## Connecting and Disconnecting

The first step to establish a connection to Kratos CoSimulation is to use the `Connect` method:

```py
# The connect should be called before any CosimIO method
info = CoSimIO.Connect(settings)
```

First of all, you may notice that `Connect` method takes a `CoSimIO::Info` as its argument. This container can be used to pass additional information about the solver/software-tool or connection settings to the _CoSimIO_:

```py
settings = CoSimIO.Info()
settings.SetString("my_name", "py_connect_disconnect_a")    # my name
settings.SetString("connect_to", "py_connect_disconnect_b") # to whom I want to connect to
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")
```

It is very important that the settings for `my_name` and `connect_to` are set correctly, otherwise the connection cannot be established!

E.g in the above example one code sets `my_name` as `py_connect_disconnect_a` and `connect_to` as `py_connect_disconnect_b`. This means that the other code has to use the same settings but reversed: `my_name` must be `py_connect_disconnect_b` and `connect_to` must be `py_connect_disconnect_a`.

This method returns a `Info` object containing information about the connection which can be queried using `GetString` method. For further calls to `CoSimIO` it is necessary to get the `connection_name`:

```py
connection_name = info.GetString("connection_name")
```

Similar things work for the `Disconnect` method.
Now putting together everything:

```py
import CoSimIO

settings = CoSimIO.Info()
settings.SetString("my_name", "py_connect_disconnect_a")    # my name
settings.SetString("connect_to", "py_connect_disconnect_b") # to whom I want to connect to
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

info = CoSimIO.Connect(settings)

# The connect_info contains now:
# - The name of the connection ("connection_name") to be used for further calls to CoSimIO
# - The status of the connection ("connection_status")

connection_name = info.GetString("connection_name") # getting name of connection for future calls
if info.GetInt("connection_status") == CoSimIO.ConnectionStatus.Connected:
    print("Connected!")

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") == CoSimIO.ConnectionStatus.Disconnected:
    print("Disconnected!")
```

This example can be found in [integration_tutorials/python/connect_disconnect_a.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/connect_disconnect_a.py) and [integration_tutorials/python/connect_disconnect_b.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/connect_disconnect_b.py).

## Data Exchange

Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Data Exchange.

One of the important missions of the CoSimIO is to send and recieve data between different solvers/software tools. The `ExportData` method can be used to send data to the Kratos or directly to another solver/software-tool:

```py
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name) # connection_name is obtained from calling "Connect"
data_to_be_export = CoSimIO.DoubleVector([3.14,3.14,3.14,3.14]) # generic Vector to avoid memory copy when going from python to C++ and back
return_info = CoSimIO.ExportData(info, data_to_be_export)
```

The `ImportData` should be used on the other side to recieve data:

```py
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
data_to_be_import = CoSimIO.DoubleVector()
return_info = CoSimIO.ImportData(info, data_to_be_import)
```

It is important to mention that `ImportData` will clear and resize the vector if needed.

This example can be found in [integration_tutorials/python/export_data.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/export_data.py) and [integration_tutorials/python/import_data.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/import_data.py).

## Mesh Exchange

Make sure to connect (refer [Connecting and Disconnecting](#connecting-and-disconnecting)) before Mesh Exchange.

After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh` method:

```py
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", connection_name) # connection_name is obtained from calling "Connect"

model_part = CoSimIO.ModelPart("name_of_model_part_to_export")

export_info = CoSimIO.ExportMesh(info, model_part)
```

The argument `model_part` is of type `CoSimIO::ModelPart` and contains a mesh. Its usage is explained [here](../../model_part/model_part_python.md).

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```py
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", "test_connection")

model_part = CoSimIO.ModelPart("name_of_imported_model_part")

import_info = CoSimIO.ImportMesh(info, model_part)
```

This example can be found in [integration_tutorials/python/export_mesh.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/export_mesh.py) and [integration_tutorials/python/import_mesh.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/import_mesh.py).

## Next steps

In the [next tutorial](basic_data_exchange_with_kratos.md), a connection to Kratos is established and basic data exchange with Kratos is done.
