# Building Kratos with CoSimulation

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

Before starting the connection to Kratos one should build the Kratos with the necessary applications. The easiest way to build the Kratos would be using the standard_configure files provided in the [scripts](https://github.com/KratosMultiphysics/Kratos/tree/master/scripts) folder. These scripts would provide the Kratos core library but not the required CoSimulation application and (optional but useful) Mapping application. For adding them you may copy for instance the standard_configure.sh to cosimulation_configure.sh and adding these two applications (you may keep or remove the rest):

```bash
# Set applications to compile
export KRATOS_APPLICATIONS=
add_app ${KRATOS_APP_DIR}/CoSimulationApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
```

Now for running the Kratos, one should configure the python environment variables as follows:
* `PYTHONPATH` should point to the Kratos binaries root (`path/to/kratos`) where you can find KratosMultiphysics sub folder
* `LD_LIBRARY_PATH` should point to the libs folder (`path/to/kratos/libs`) where you can find compiled libraries.

## Building Kratos with MPI support

For mpi-distributed coupled simulations it is necessary to compile Kratos with MPI support. This can be done by setting the CMake configure flag:
```
-DUSE_MPI=ON
```

See also [here](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md#parallelism)

For more information about the Kratos build requirements, options please check the [Kratos install guide](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md).
