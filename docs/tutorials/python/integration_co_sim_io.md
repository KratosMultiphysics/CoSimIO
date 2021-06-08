# Tutorial for integrating the _CoSimIO_ using the Python interface

This tutorial helps you to integrate the _CoSimIO_ into a solver/software-tool using the Python interface.

<!--
## Overview

- [Tutorial for integrating the _CoSimIO_ using the Python interface](#tutorial-for-integrating-the-cosimio-using-the-python-interface)
  - [What you need](#what-you-need)
  - [Tutorial 1: Building the CoSimIO](#tutorial-1-building-the-cosimio)
  - [Tutorial 2: Hello CosimIO](#tutorial-2-hello-cosimio)
  - [Tutorial 3: Connecting and Disconnecting](#tutorial-3-connecting-and-disconnecting)
  - [Tutorial 4: Data Exchange](#tutorial-4-data-exchange)
  - [Tutorial 5: Mesh Exchange](#tutorial-5-mesh-exchange)
  - [Tutorial 6: Kratos CoSimulation Library Overview](#tutorial-6-kratos-cosimulation-library-overview)
  - [Tutorial 7: Building Kratos with CoSimulation](#tutorial-7-building-kratos-with-cosimulation)





- [Tutorial for integrating the _CoSimIO_ using the Python interface](#tutorial-for-integrating-the-cosimio-using-the-python-interface)
  - [What you need](#what-you-need)
  - [Tutorial 1: Building the CoSimIO](#tutorial-1-building-the-cosimio)
  - [Tutorial 2: Hello CosimIO](#tutorial-2-hello-cosimio)
  - [Tutorial 3: Connecting and Disconnecting](#tutorial-3-connecting-and-disconnecting)
  - [Tutorial 4: Data Exchange](#tutorial-4-data-exchange)
  - [Tutorial 5: Mesh Exchange](#tutorial-5-mesh-exchange)
  - [Tutorial 6: Kratos CoSimulation Library Overview](#tutorial-6-kratos-cosimulation-library-overview)
  - [Tutorial 7: Building Kratos with CoSimulation](#tutorial-7-building-kratos-with-cosimulation)
-->

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler. This includes most of the major C++ compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)



## Tutorial 1: Building the CoSimIO
The Python interface is located in [co_sim_io/python](https://github.com/KratosMultiphysics/CoSimIO/tree/master/co_sim_io/python) folder of the repository. It uses the [pybind11 library](https://github.com/pybind/pybind11) for exposing the C++ code to Python. It is provided in the [expternal_libraries/pybind11](https://github.com/KratosMultiphysics/CoSimIO/tree/master/external_libraries/pybind11) folder of this repo.

Using cmake the compilation is straightforward. Just calling the following command from the _CoSimIO_ root:

```
sh scripts/build_python.sh
```

This will compile the binaries in the build folder and installs them into the bin subfolder of _CoSimIO_.

Please don't forget to add the binaries in the bin folder to your `PYTHONPATH`.

Usually pybind automatically detects the Python installation. Sometimes it can however be necessary to explicitly specify the target Python version. This can be achieved through CMake by setting PYBIND11_PYTHON_VERSION or an exact Python installation can be specified with PYTHON_EXECUTABLE. For example:
```cmake
-DPYBIND11_PYTHON_VERSION=3.6
# or
-DPYTHON_EXECUTABLE=path/to/python
```
see also the corresponding [pybind documentstion](https://pybind11.readthedocs.io/en/stable/compiling.html#configuration-variables)

If Python cannot find the _CoSimIO_ then check if the version of Python used for the compilation is the same as the version of Python used. E.g. when compiling _CoSimIO_ with Python 3.6 it is not possible to import it when using Python 3.8. Here Python issues a `ModuleNotFoundError`.
This can easily be checked with the file extension of the compiled _CoSimIO_ module.
Example: When using Python 3.5 the name of the compiled module will be sth like `CoSimIO.cp35-win_amd64.pyd` (Windows), `CoSimIO.cpython-35m-x86_64-linux-gnu.so` (Linux) or `CoSimIO.cpython-35m-darwin.so` (MacOS).


## Tutorial 2: Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```python
import CoSimIO
info = CoSimIO.Hello()
```
Please note that this method like other methods in _CoSimIO_ returns an `Info` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```python
print(info)
```

Info class also provides get method for retriving a certain value knowing its type:

```python
major_version = info.GetInt("major_version")
minor_version = info.GetInt("minor_version")
patch_version = info.GetString("patch_version")
```

This example can be found in [integration_tutorials/python/hello.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/hello.py).



## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:
```python
# The connect should be called before any CosimIO method called
info = CoSimIO.Connect(settings)
```

First of all, you may notice that `Connect()` method takes an `Info` as its arguments. This container can be used to pass additional information about the solver/software-tool or connection settings to the _CoSimIO_:

```python
settings = CoSimIO.Info()
settings.SetString("my_name", "cpp_connect_disconnect_a") # my name
settings.SetString("connect_to", "cpp_connect_disconnect_b") # to whom I want to connect to
settings.SetInt("echo_level", 1)
settings.SetString("version", "1.25")
```
This method returns a `Info` object containing information about the connection which can be queried using Get method. For further calls to `CoSimIO` it is necessary to get the `connection_name`:

```py
connection_name = info.GetString("connection_name")
```

Now putting together everything:

```python
import CoSimIO

settings = CoSimIO.Info()
settings.SetString("my_name", "cpp_connect_disconnect_a") # my name
settings.SetString("connect_to", "cpp_connect_disconnect_b") # to whom I want to connect to
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

info = CoSimIO.Connect(settings)
std::connection_name = info.GetString("connection_name") # getting name of connection for future calls
if info.GetInt("connection_status") == CoSimIO.ConnectionStatus.Connected:
    print("Connected!")

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)
info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") == CoSimIO.ConnectionStatus.Disconnected:
    print("Disconnected!")
```

This example can be found in [integration_tutorials/python/connect_disconnect_a.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/connect_disconnect_a.py) and [integration_tutorials/python/connect_disconnect_b.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/connect_disconnect_b.py).



## Tutorial 4: Data Exchange
One of the important missions of the _CoSimIO_ is to send and recieve data between processes. The `ExportData()` method can be used to send data to the Kratos or directly to another solver/software-tool:

```python
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name) # connection_name is obtained from calling "Connect"
data_to_be_export = CoSimIO.DoubleVector([3.14,3.14,3.14,3.14]) # generic Vector to avoid memory copy when going from python to C++ and back
return_info = CoSimIO.ExportData(info, data_to_be_export)
```
The `ImportData()` should be used on the other side to import data:

```python
info = CoSimIO.Info()
info.SetString("identifier", "vector_of_pi")
info.SetString("connection_name", connection_name)
data_to_be_import = CoSimIO.DoubleVector()
return_info = CoSimIO.ImportData(info, data_to_be_import)
```

This example can be found in [integration_tutorials/python/export_data.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/export_data.py) and [integration_tutorials/python/import_data.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/import_data.py).



## Tutorial 5: Mesh Exchange
After seeing how we transfer raw data between solvers/software-tools, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```Python
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", connection_name) # connection_name is obtained from calling "Connect"

model_part = CoSimIO.ModelPart("name_of_model_part_to_export");

export_info = CoSimIO.ExportMesh(info, model_part)
```

The argument `model_part` is of type `CoSimIO.ModelPart`. Its usage is explained [here](../../model_part/model_part_cpp.md).

On the other side one can use the ImportMesh() method to get the mesh sent by the export:

```Python
info = CoSimIO.Info()
info.SetString("identifier", "fluid_mesh")
info.SetString("connection_name", "test_connection")

model_part = CoSimIO.ModelPart("name_of_imported_model_part");

import_info = CoSimIO.ImportMesh(info, model_part)
```

This example can be found in [integration_tutorials/python/export_mesh.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/export_mesh.py) and [integration_tutorials/python/import_mesh.py](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/tests/integration_tutorials/python/import_mesh.py).


## Tutorial 6: Kratos CoSimulation Library Overview
The overview of the Kratos CoSimulation Library can be found [here](../index.md#kratos-cosimulation-library-overview).


## Tutorial 7: Building Kratos with CoSimulation
The building instructions for the Kratos CoSimulation Library can be found [here](../index.md#building-kratos-with-cosimulation).

<!--
## Tutorial 8: Connecting/Disconnecting to/from Kratos
coming soon!


## Tutorial 9: Data Exchange with Kratos
coming soon!


## Tutorial 10: Mesh Exchange with Kratos
coming soon!


## Tutorial 11: Mapping with Kratos
coming soon! -->
