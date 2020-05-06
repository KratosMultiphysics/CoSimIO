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

## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"

int main(){
    const std::string connection_name = "external_solver"; // this is different for every solver
    ConnectionSettings settings;
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto info = CoSimIO::Connect(connection_name, );


    // ...


    auto info = CoSimIO::Disconnect(connection_name);
    return 0;
}
```