# CoSimIO with MPI support (C++ interface)

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Building the CoSimIO with MPI support](#building-the-cosimio-with-mpi-support)
- [Number of processes](#number-of-processes)
- [Connecting](#connecting)
- [Importing and Exporting](#importing-and-exporting)

<!-- /code_chunk_output -->
---

This document describes the C++ MPI interface of the CoSimIO for performing MPI-parallel coupled simulations. Most functionalities work in the same way in serial and MPI, the differences are pointed out and explained here.

## Building the CoSimIO with MPI support
Please check the [serial instructions](integration_co_sim_io.md#building-the-cosimio) first.

The _CoSimIO_ has to be built with MPI support in order to perform mpi-parallel distributed coupled simulations. For this an MPI implementation (e.g OpenMPI or IntelMPI) is necessary.
Set the CMake option `CO_SIM_IO_BUILD_MPI` to `ON` in order to compile the CoSimIO with MPI support.

One can use [build_cpp.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_cpp.sh) for compiling it. Check [here](../../build_options.md) for the available build options.

```bash
$ bash scripts/build_cpp.sh
```

The shared library `co_sim_io_mpi` will be installed in the `bin/` folder (note that `co_sim_io_mpi` already links against `co_sim_io` and MPI). After building and linking it to your project, you may use the interface defined in `co_sim_io_mpi.hpp` (which already includes `co_sim_io.hpp`):

```c++
// CoSimulation includes
#include "co_sim_io_mpi.hpp"

int main()
{
    return 0;
}
```

With CMake this can be achieved with the following:
```
include_directories(path/to/co_sim_io)
target_link_libraries(my_executable co_sim_io_mpi)
```

## Number of processes
Before going into the details of the interface, it is important to clarify that both connection partners must use the same number of processes! This means that each rank on one side has its dedicated partner rank on the other side of the interface.

## Connecting
Please check the [serial instructions](integration_co_sim_io.md#connecting-and-disconnecting) first.

For using the _CoSimIO_ in an mpi-parallel context, it is required to establish a connection with `ConnectMPI` instead of `Connect`. This is the first step to establish a connection to Kratos CoSimulation. Besides the settings, it takes an `MPI_Comm` communicator. This communicator should only have the ranks that contain the interface. It is also possible for it to contain all the ranks (e.g. `MPI_COMM_WORLD`) when some ranks are empty i.e. have no part of the coupling interface, but this is less efficient.
```c++
// The connect must be called before any CosimIO method
// mpi_comm_interface should only contain the ranks that have a part of the coupling interface
auto info = CoSimIO::ConnectMPI(settings, mpi_comm_interface);
```

Note that it is required to initialize MPI before calling `ConnectMPI`, e.g. with `MPI_Init`.

Everything else related to connecting and disconnecting works the same as in serial.

## Importing and Exporting
The functions for importing and exporting don't change, neither in terms of interface nor in terms of functionality. As explained [above](number-of-processes), both connection partners must use the same number of processes, and hence the ranks can directly communicate with each other.

Examples can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/mpi).

One minor difference is that `ModelParts` can have ghost nodes in case they are distributed across multiple ranks. See [here](../../model_part/model_part_cpp.md#interface-for-distributed-modelparts-mpi) for more details. The does not change the inteface of `Im-/ExportMesh`.
