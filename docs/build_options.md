# Build options for the _CoSimIO_

The CoSimIO uses [CMake](https://cmake.org/) as build system. The following options are available for configuration:

| name | default| description |
|---|---|---|
| CO_SIM_IO_BUILD_TYPE | `CMAKE_BUILD_TYPE` (in case it is defined), otherwise `Release` | Build type with which the _CoSimIO_ is built. Use `Release` for production and `Debug` to enable more runtime checks (at the cost of being slower).  |
| CMAKE_INSTALL_PREFIX | source directory of CoSimIO | Used to specify the install directory. Binaries will be installed in `${CMAKE_INSTALL_PREFIX}/bin` |
| CO_SIM_IO_BUILD_TESTING | ON (same as `BUILD_TESTING`) | Build the tests. Useful for developing and initial installation to check if everything works as expected. |
| CO_SIM_IO_BUILD_MPI | OFF | Enable MPI support for distributed environments. Requires an installation of [MPI](https://www.mpi-forum.org/). |
| CO_SIM_IO_BUILD_C | OFF | Build the C-interface |
| CO_SIM_IO_BUILD_PYTHON | OFF | Build the Python-interface |
| CO_SIM_IO_STRICT_COMPILER | OFF | Enable more warnings in the compiler, useful for development. |

---

The configuration that is used is also printed when compiling, e.g.

~~~sh
Building the CoSimIO with the following configuration:
    CO_SIM_IO_BUILD_TYPE:      Release
    CO_SIM_IO_BUILD_MPI:       ON
    CO_SIM_IO_BUILD_TESTING:   ON
    CO_SIM_IO_BUILD_C:         ON
    CO_SIM_IO_BUILD_PYTHON:    ON
    CO_SIM_IO_STRICT_COMPILER: ON
~~~
