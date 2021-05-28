# Tutorials for integrating the _CoSimIO_

This folder contains step-by-step instructions on how to integrate the _CoSimIO_ into a solver/software-tool. Since the _CoSimIO_ is independent of Kratos, this is done completely standalone.

Each subfolder has the tutorial for an specific language:
* **C++ Interface**: Is the natural interface of the _CoSimIO_ providing its full capabilities.
* **Python Interface**: Is an extensive interface based on the C++ one which provides the most capabilities.
* **C Interface**: A subset of the C++ interface providing the interface for double and common C arrays with set of functions representing the C++ class methods.

The Python interface is recommended for solvers/software-tools implemented in Python. Furthermore it is recommend to be used if the solver/software-tool has an efficient Python interface (efficient in the sense of compute and memory-overhead). The C++ is the most complete one and the C is a practical substitute for solvers/software-tools in other languages avoiding the incompatibilities and complexities of C++ interface.


## Kratos CoSimulation
For using the _CoSimIO_ with Kratos it is necessary to build Kratos (separately). Basic explanations are given below with links for more detailed descriptions.

## Kratos CoSimulation Library Overview
Conceptually the Kratos CoSimulation consists of 3 main components:
1. **Data Transfer**: Any co-simulation process needs several data communications between different solvers/executables and Kratos. This includes raw data (in the form of vectors), meshes, and mesh data (like nodal and elemental data) or control signals. So a flexible data transfer mechanism one of the important components of the library.
2. **Coupling Solutions**: Coupling algorithms have many common parts like the strategy, convergence criteria, convergence accelerators, and predictors. To be seamlessly usable in Kratos such an algorithm should provide the same interface as internal solutions of Kratos. The Kratos CoSimulation provides many of these tools and algorithms generic enough to be used in different contexts.
3. **Mapping Between Meshes**: In many coupling problems the mesh in each domain is done considering the special requirements of that domain (like fluid, structure, etc.). This means that in those cases the meshes are not conformant between does domains and we should map our data from one to the other. Depending on the type of the coupling this may happen over some surfaces or volumes of the model. Kratos CoSimulation also provides this capability.

Based on these concepts these are the main components of the library:
1. **CoSimIO**: This library is in charge of the data transfer between different programs using inter process communication.
2. **CoSimulationApplication**: This is an application of Kratos which is located in [kratos/applications/CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication) folder in the Kratos repository. This application provides a generic coupled solver with several convergence criteria, convergence accelerators, and predictors. Check the link for a more detailed overview.
3. **MappingApplication**: The standard mapping application of the Kratos located in [kratos/applications/MappingApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication) folder. This application provides the mapping of data from one mesh to another one. It supports the 1D, 2D and 3D mapping with different types of mappings (like nearest node, nearest element, etc.). Check the link for a more detailed overview.

Kratos provides an extensive Python interface to its CoSimulation library which is used in the tutorials.


## Building Kratos with CoSimulation
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
