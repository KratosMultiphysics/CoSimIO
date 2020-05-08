# Tutorial for integrating the _CoSimIO_ using CPP interface

This tutorial helps you through to integrate the _CoSimIO_ into a solver using the CPP interface.

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers in https://en.cppreference.com/w/cpp/compiler_support#cpp11


## Tutorial 1: Building
The CPP version of CosimIO is header only and for building you may only include the [co_sim_io.h](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/co_sim_io.hpp) in your project file and compile it with your code:

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

Please note that this method like other methods in _CoSimIO_ returns a `ReturnInfo` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```c++
std::cout << info << std::endl;
```

Info class also provides get method for retriving a certain value knowing its type:

```c++
int major_version = info.Get<int>("major_version");
int minor_version = info.Get<int>("minor_version");
std::string patch_version = info.Get<std::string>("patch_version");
```
You may find this example in hello.cpp file in the `solver_integration/cpp` folder


## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:
```c++
// The connect should be called before any CosimIO method called
auto info = CoSimIO::Connect(connection_name, settings);
}
```

First of all, you may notice that `Connect()` method takes a `ConnectionSettings` as its arguments. This contianer has the interface like the Info described in the previous section and can be used to pass additional information about the solver or connection settings to the CoSimIO:

```c++
CoSimIO::ConnectionSettings settings;
settings.Set("connection_name", "test_connection"); // This should be unique for each connection between two solvers
settings.Set("solver_name", "my_solver"); // Not to be confused with the connection name. 
settings.Set("echo_level", 1);
settings.Set("solver_version", "1.25");
}
```
This method returns a `ReturnInfo` object containing information about the connection which can be queried using Get method:

```c++
int connection_status = info.Get<int>("connection_status");
```

Now putting together everything:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"
int main(){
    CoSimIO::ConnectionSettings settings;
    settings.Set("connection_name", "test_connection"); // This should be unique for each connection between two solvers
    settings.Set("solver_name", "my_solver"); // Not to be confused with the connection name. 
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto return_info = CoSimIO::Connect(settings);
    if(return_info.Get<int>("connection_status") != CoSimIO::Connected) 
        return 1;
    // Now you may call any CoSimIO methods like ImportData, ExportData, etc.

    // ...
    return_info = CoSimIO::Disconnect(settings); // disconnect afterwards
    // Here you may use the return_info but cannot call any CoSimIO method anymore
    if(return_info.Get<int>("connection_status") != 0) 
        return 1;
    
    return 0;
}
```

You may find this example in connect_disconect.cpp file in the `solver_integration/cpp` folder

## Tutorial 4: Data Exchange
One of the important missions of the CoSimIO is to send and recieve data between processes. The `ExportData()` method can be used to send data to the Kratos or directly to another solver:

```c++
std::vector<double> data_to_send(4,3.14);
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", "solver_2");
auto return_info = ExportData(info, data_to_send);
```
The `ImportData()` should be used on the other side to recieve data:

```c++
std::vector<double> data_to_receive;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", "solver_1");
auto return_info = ImportData(info, data_to_receive);
```

It is important to mention that the `ImportData()` will clear and resize the vector if needed.


## Tutorial 5: Mesh Exchange
In the previous tutorial we have seen how to export and import data a


## Tutorial 6: Building the Kratos CoSimApplication


## Tutorial 7: Connecting to Kratos CoSimApplication



