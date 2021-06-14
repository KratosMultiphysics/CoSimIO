~~~
   ______     _____ _           ________
  / ____/___ / ___/(_)___ ___  /  _/ __ |
 / /   / __ \\__ \/ / __ `__ \ / // / / /
/ /___/ /_/ /__/ / / / / / / // // /_/ /
\____/\____/____/_/_/ /_/ /_/___/\____/
Kratos CoSimulationApplication
~~~

[![CI](https://github.com/KratosMultiphysics/CoSimIO/actions/workflows/ci.yml/badge.svg?event=schedule)](https://github.com/KratosMultiphysics/CoSimIO/actions/workflows/ci.yml)
# CoSimIO

The _CoSimIO_ is a small library for interprocess communication in CoSimulation contexts. It is designed for exchanging data between different solvers or other software-tools. For performing coupled simulations it is used in combination with the [_CoSimulationApplication_](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication).\
It is implemented as a _detached interface_. This means that it follows the interface of Kratos but is independent of Kratos, which allows for an easy integration into other codes / solvers

The implementation is defined in [co_sim_io](co_sim_io)

Documentation and tutorials can be found [here](https://kratosmultiphysics.github.io/CoSimIO/)

The tests are contained in [tests](tests)

## Dependencies
- The _CoSimIO_ is implemented as a standalone tool in C++. The only dependency is C++11. This includes most of the major C++ compilers in Windows, Linux, and Mac. A detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11).
- Dependency on Kratos: There is **NO** dependency on Kratos. The _CoSimIO_ can be used completely without including or linking against Kratos.
- _CoSimIO_ is **header-only**, no compiling/linking is required.
- The CoSimIO uses [std::filesystem](https://en.cppreference.com/w/cpp/filesystem) which is part of C++17. When compiling with C++11, the [C++11 compatible version of std::filesystem](https://github.com/gulrak/filesystem) is used.
   Note that in Windows this includes the `windows.h` header. For details check [here](co_sim_io/impl/filesystem_inc.hpp).

## Available languages
Besides the native C++ interface, the _CoSimIO_ also provides interfaces to other languages. Currently the following languages are supported:
- C
- Python

These interfaces are implemented as consistent as possible with the C++ interface.

## Usage
This section provides a brief overview on how to use the _CoSimIO_. For more detailed explanations please check the [documentation](https://kratosmultiphysics.github.io/CoSimIO/) in the corresponding language.

### **C++**
The C++ interface is defined in [co_sim_io.hpp](co_sim_io/co_sim_io.hpp). It is sufficient to include this header, no compilation/linking is necessary because it is header-only.

### **C**
The C interface is defined in [co_sim_io_c.h](co_sim_io/c/co_sim_io_c.h). In addition to including this header it is required to compile [co_sim_io_c.c](co_sim_io/c/co_sim_io_c.c) into a shared library and link against it.

### **Python**
The Python interface is defined in [_CoSimIO_ python module](co_sim_io/python/co_sim_io_python.cpp). The [pybind library](https://github.com/pybind/pybind11) is used for the Python exposure of the C++ interface.

## Parallelism
Currently the _CoSimIO_ runs purely sequential. In the future capabilities for distributed MPI simulations are planned to be implemented.
Shared memory parallelism is currently not planned but might be added at a later stage.

## Memory Management
Coupling requires frequent exchange of data. Therefore the _CoSimIO_ uses the memory provided by the including solvers/software-tools directly, without copying it to an auxiliar data structure.
