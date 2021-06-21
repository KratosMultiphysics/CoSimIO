# Building Kratos with CoSimulation
Before starting the connection to Kratos we should build the Kratos with the necessary applications. The easiest way to build the Kratos would be using the standard_configure files provided in the [scripts](https://github.com/KratosMultiphysics/Kratos/tree/master/scripts) folder. These scripts would provide the Kratos core library but not the required CoSimulatation application and (optional but useful) Mapping application. For adding them you may copy for instance the standard_configure.sh to cosimulation_configure.sh and adding these two applications (you may keep or remove the rest):

```bash
# Set applications to compile
export KRATOS_APPLICATIONS=
add_app ${KRATOS_APP_DIR}/CoSimulationApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
```

Now for running the Kratos, first you should add configure the python environment variables pointing to Kratos folder:
* `PYTHONPATH` should point to the Kratos binaries root (`path/to/kratos`) where you can find KratosMultiphysics sub folder
* `LD_LIBRARY_PATH` should point to the libs folder (`path/to/kratos/libs`) where you can find compiled libraries.

For more information about the Kratos build requirements, options please check the [Kratos install guide](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md)

