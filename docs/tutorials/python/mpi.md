# CoSimIO with MPI support (Python interface)

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

This document describes the Python MPI interface of the CoSimIO for performing MPI-parallel coupled simulations. Most functionalities work in the same way in serial and MPI, the differences are pointed out and explained here.

## Building the CoSimIO with MPI support
Please check the [serial instructions](integration_co_sim_io.md#building-the-cosimio) first.

The _CoSimIO_ has to be built with MPI support in order to perform mpi-parallel distributed coupled simulations. For this an MPI implementation (e.g OpenMPI or IntelMPI) is necessary.
Set the CMake option `CO_SIM_IO_BUILD_MPI` to `ON` in order to compile the CoSimIO with MPI support.

One can use [build_python.sh](https://github.com/KratosMultiphysics/CoSimIO/blob/master/scripts/build_python.sh) for compiling it. Check [here](../../build_options.md) for the available build options.
Additionally the interface for [mpi4py](https://pypi.org/project/mpi4py/) can be enabled with the CMake flag `CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE`. This allows to pass mpi4py communicators as explained [below](#connecting).

```bash
$ bash scripts/build_python.sh
```

This will compile the binaries and install them into the bin/ subfolder of CoSimIO, together with the CoSimIO Python module.

The created folder structure should look like this:
```
| - bin
  | - PyCoSimIO.cpython-35m-x86_64-linux-gnu.so # (Linux version)
  | - PyCoSimIOMPI.cpython-35m-x86_64-linux-gnu.so # (Linux version)
  | - PyCoSimIOMPI_mpi4pyInterface.cpython-35m-x86_64-linux-gnu.so # (Linux version) # if enabled with "CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE"
    - CoSimIO
    | - __init__.py
    | - mpi
      | - __init__.py
      | - mpi4pyInterface # if enabled with "CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE"
        | - __init__.py
```

## Number of processes
Before going into the details of the interface, it is important to clarify that both connection partners must use the same number of processes! This means that each rank on one side has its dedicated partner rank on the other side of the interface.

## Connecting
Please check the [serial instructions](integration_co_sim_io.md#connecting-and-disconnecting) first.

For using the _CoSimIO_ in an mpi-parallel context, it is required to establish a connection with `ConnectMPI` instead of `Connect`. This is the first step to establish a connection to Kratos CoSimulation. As MPI does not have a native Python interface, one version of this function takes only the settings as input and internally uses `MPI_COMM_WORLD`.
```py
# The connect must be called before any CosimIO method
from CoSimIO.mpi import ConnectMPI
info = ConnectMPI(settings) # internally uses MPI_COMM_WORLD
```

If the inteface to mpi4py is enabled, then it is also possible to pass an mpi4py communicator to `ConnectMPI`:
```py
# The connect must be called before any CosimIO method
from CoSimIO.mpi import ConnectMPI
from CoSimIO.mpi.mpi4pyInterface import mpi4pyCommHolder
from mpi4py import MPI

info = ConnectMPI(settings, mpi4pyCommHolder(MPI.COMM_WORLD))
```

Note that it is required to initialize MPI before calling `ConnectMPI`, e.g. with `MPI_Init`.

Everything else related to connecting and disconnecting works the same as in serial.

## Importing and Exporting
The functions for importing and exporting don't change, neither in terms of interface nor in terms of functionality. As explained [above](number-of-processes), both connection partners must use the same number of processes, and hence the ranks can directly communicate with each other.

Examples can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/python/mpi).

One minor difference is that `ModelParts` can have ghost nodes in case they are distributed across multiple ranks. See [here](../../model_part/model_part_python.md#interface-for-distributed-modelparts-mpi) for more details. The does not change the inteface of `Im-/ExportMesh`.
