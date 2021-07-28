# Documentation of the _CoSimIO_

Welcome to the documentation of _CoSimIO_.

## Structure of _CoSimIO_ and which part it plays in Kratos CoSimulation
Even though the _CoSimIO_ is a standalone tool, it reaches its full capabilities only in combination with the CoSimulation features of Kratos. It is implemented as _detached interface_. This section explains how everything fits together.

### Kratos CoSimulation Library Overview
Conceptually the Kratos CoSimulation consists of 3 main components:
1. **Data Transfer**: Any co-simulation process needs several data communications between different solvers/executables and Kratos. This includes raw data (in the form of vectors), meshes, and mesh data (like nodal and elemental data) or control signals. So a flexible data transfer mechanism one of the important components of the library.
2. **Coupling Solutions**: Coupling algorithms have many common parts like the strategy, convergence criteria, convergence accelerators, and predictors. To be seamlessly usable in Kratos such an algorithm should provide the same interface as internal solutions of Kratos. The Kratos CoSimulation provides many of these tools and algorithms generic enough to be used in different contexts.
3. **Mapping Between Meshes**: In many coupling problems the mesh in each domain is done considering the special requirements of that domain (like fluid, structure, etc.). This means that in those cases the meshes are not conformant between does domains and we should map our data from one to the other. Depending on the type of the coupling this may happen over some surfaces or volumes of the model. Kratos CoSimulation also provides this capability.

Based on these concepts these are the main components of the library:
1. **CoSimIO**: This library is in charge of the data transfer between different programs using interprocess communication.
2. **CoSimulationApplication**: This is an application of Kratos which is located in [kratos/applications/CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication) folder in the Kratos repository. This application provides a generic coupled solver with several convergence criteria, convergence accelerators, and predictors. Check the link for a more detailed overview.
3. **MappingApplication**: The standard mapping application of the Kratos located in [kratos/applications/MappingApplication](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/MappingApplication) folder. This application provides the mapping of data from one mesh to another one. It supports the 1D, 2D and 3D mapping with different types of mappings (like nearest node, nearest element, etc.). Check the link for a more detailed overview.

Kratos provides an extensive Python interface to its CoSimulation library which is used in the tutorials.


## Tutorials for integrating the _CoSimIO_
The tutorials can be found [here](tutorials/README.md).

## API documentation
The documentation for the API of the _CoSimIO_ can be found [here](api_docs/README.md).

## Communication
The available methods for (interprocess) communication are explained [here](communication.md).

## Kratos CoSimulation
For using the _CoSimIO_ with Kratos it is necessary to build Kratos (separately). Basic explanations are given [here](kratos_build.md) with links for more detailed descriptions.

## Miscellaneous
- [Mesh Connectivities](mesh_connectivities.md)
- Documentation of `CoSimIO::Info`: [link](info/info_cpp.md)
- Documentation of `CoSimIO::ModelPart`: [link](model_part/model_part_cpp.md)
