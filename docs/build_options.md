The CoSimIO uses [CMake](https://cmake.org/) as build system. The following options are available for configuration:


- CMAKE_BUILD_TYPE
- CO_SIM_IO_ENABLE_MPI
- BUILD_TESTING
- CO_SIM_IO_BUILD_C
- CO_SIM_IO_BUILD_PYTHON
- CO_SIM_IO_STRICT_COMPILER


OPTION ( CO_SIM_IO_ENABLE_MPI         "Enable MPI communication and MPI examples" OFF )
OPTION ( CO_SIM_IO_BUILD_C            "Building the CoSimIO for C"                OFF )
OPTION ( CO_SIM_IO_BUILD_PYTHON       "Building the CoSimIO for Python"           OFF )
OPTION ( CO_SIM_IO_BUILD_FORTRAN      "Building the CoSimIO for Fortran"          OFF )
OPTION ( CO_SIM_IO_STRICT_COMPILER    "Compiler has more warnings"                OFF )