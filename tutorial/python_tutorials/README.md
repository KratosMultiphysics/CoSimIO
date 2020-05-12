# Tutorial for integrating the _CoSimIO_ using the Python interface

This tutorial helps you through to integrate the _CoSimIO_ into a solver using the Python interface.

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)


## Tutorial 1: Building
The Python interface is located in [co_sim_io/python](https://github.com/KratosMultiphysics/CoSimIO/tree/master/co_sim_io/python) folder of the repository. It depends to the pybind11 library which is provided in the [expternal_libraries/pybind11](https://github.com/KratosMultiphysics/CoSimIO/tree/master/external_libraries/pybind11) folder of the repo.

Using cmake the compilation is straightforward. Just calling the following command from the CoSimIO root:

```
sh scripts/build_python.sh
```

This will compile the binaries in the build folder and installs them into the bin subfolder of CoSimIO.

Please don't forget to add the binaries in the bin folder to your `PYTHONPATH`.

## Tutorial 2: Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```python
import CoSimIO
return_info = CoSimIO.Hello()
```
Please note that this method like other methods in _CoSimIO_ returns a `ReturnInfo` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```python
print(return_info)
```

Info class also provides get method for retriving a certain value knowing its type:

```python
major_version = return_info.GetInt("major_version")
minor_version = return_info.GetInt("minor_version")
patch_version = return_info.GetString("patch_version")
```
You may find this example in hello.py file in the `solver_integration/python_solver` folder

## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:
```python
# The connect should be called before any CosimIO method called
return_info = CoSimIO.Connect(settings)
```

First of all, you may notice that `Connect()` method takes a `ConnectionSettings` as its arguments. This contianer has the interface like the Info described in the previous section and can be used to pass additional information about the solver or connection settings to the CoSimIO:

```python
settings = CoSimIO.ConnectionSettings()
settings.SetString("connection_name", "test_connection") # This should be unique for each connection between two solvers
settings.SetString("solver_name", "my_solver") # Not to be confused with the connection name. 
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")
```
This method returns a `ReturnInfo` object containing information about the connection which can be queried using Get method:

```python
return_info.GetInt("connection_status")
```

Now putting together everything:

```python
import CoSimIO

settings = CoSimIO.ConnectionSettings()
settings.SetString("connection_name", "test_connection") # This should be unique for each connection between two solvers
settings.SetString("solver_name", "my_solver") # Not to be confused with the connection name. 
settings.SetInt("echo_level", 1)
settings.SetString("solver_version", "1.25")

return_info = CoSimIO.Connect(settings)
if return_info.GetInt("connection_status") == 1:
    print("Connected!")

return_info = CoSimIO.Disconnect(settings)
if return_info.GetInt("connection_status") == 0:
    print("Disconnected!")
}
```

You may find this example in connect_disconect.cpp file in the `solver_integration/cpp` folder






