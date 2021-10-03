# Changelog

All important and notable changes in the _CoSimIO_ are documented in this file.

## 2.0.0
- Changes in Connecting:
    - Now instead of specifying `connection_name` directly, `my_name` and `connect_to` are used for the call to `Connect`.
    - The `CoSimIO::Info` that is returned from `Connect` now contains the `connection_name` that needs to be used in subsequent calls to _CoSimIO_.
    - This change was done to make the connection more robust and to make the selection of primary/secondary partner clear. This is only used internally e.g. for who opens a port and who connects to it.
- Introduced `CoSimIO::ModelPart` for exchanging of meshes
    - Simplified version of [`Kratos::ModelPart`](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h)
    - Simplifies and unifies the usage of `Import-/ExportMesh`
    - See the tutorials on how to use it:
        - [C++](https://kratosmultiphysics.github.io/CoSimIO/model_part/model_part_cpp.html)
        - [C](https://kratosmultiphysics.github.io/CoSimIO/model_part/model_part_c.html)
        - [Python](https://kratosmultiphysics.github.io/CoSimIO/model_part/model_part_python.html)
- FileCommunication:
    - By default now done in folder. This way leftovers from previous simulations can be easily deleted (done automatically).
    - working directory can be specified
    - stability of initial connection was significantly improved.
- Python interface: Data is no longer copied when going from Python to C++ and vice versa.
    - `Import-/ExportData` now uses `CoSimIO::DoubleVector` (small wrapper around [`std::vector`](https://en.cppreference.com/w/cpp/container/vector))
    - `Import-/ExportMesh` now uses `CoSimIO::ModelPart`
- Continuous Integration:
    - Adding Python 3.9 (now has Python v3.5 - v3.9)
    - Adding CentOS 7 build with GCC 4.8.5
    - Enforcing C89 standard

- Many improvements and cleanups under the hood

## 2.0.1
- Bugfix in remote-controlled CoSimulation (now settings can be passed to the registered functions)
- For this the `Info` object can now hold `Info` objects itself, hence making it possible to build hierarchical structures.

## 3.0.0
- Extensive documentation was added: https://kratosmultiphysics.github.io/CoSimIO/
- Now the C++ version of CoSimIO is not header-only any more. Check [here](https://kratosmultiphysics.github.io/CoSimIO/build_options.html) for the available build options.
- The CoSimIO supports MPI-parellism now
- Communication via pipes was added (currently only supported in Linux). See [here](https://kratosmultiphysics.github.io/CoSimIO/communication.html#pipe-based-communication) for the details
- The `ModelPart` interface was extended to support ghost nodes. See [here](https://kratosmultiphysics.github.io/CoSimIO/model_part/model_part_cpp.html#interface-for-distributed-modelparts-mpi) for details
