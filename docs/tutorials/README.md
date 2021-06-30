# Tutorials for integrating the _CoSimIO_

The tutorials contain step-by-step instructions for integrating the _CoSimIO_, establishing a connection to Kratos, doing mapping with Kratos and performing CoSimulation with Kratos.

Each language has its specific tutorials:

## C++ Interface
Is the natural interface of the _CoSimIO_ providing its full capabilities.

The first three tutorials show the basics:
1. Integration of _CoSimIO_ [link](cpp/integration_co_sim_io.md)
2. Basic data exchange with Kratos [link](cpp/basic_data_exchange_with_kratos.md)
3. Mesh exchange with Kratos [link](cpp/mesh_exchange_with_kratos.md)

After the basics are done, one can do:
- Mapping with Kratos (without CoSimulation) [link](cpp/mapping.md)
- CoSimulation with Kratos [link](kratos_co_simulation.md)
    - "Classical" approach [link](undefined.md)
    -  Remote controlled CoSimulation [link](undefined.md)


## C Interface
A subset of the C++ interface providing the interface for double and common C arrays with set of functions representing the C++ class methods.
1. Integration of _CoSimIO_ [link](c/integration_co_sim_io.md)

<!--
2. Basic data exchange with Kratos [link](c/basic_data_exchange_with_kratos.md)
3. Data and mesh exchange exchange with Kratos and mapping [link](c/data_and_mesh_exchange_and_mapping.md)
4. CoSimulation with Kratos [link](kratos_co_simulation.md)
-->

## Python Interface
Is an extensive interface based on the C++ one which provides the most capabilities.
1. Integration of _CoSimIO_ [link](python/integration_co_sim_io.md)

<!--
2. Basic data exchange with Kratos [link](python/basic_data_exchange_with_kratos.md)
3. Data and mesh exchange exchange with Kratos and mapping [link](python/data_and_mesh_exchange_and_mapping.md)
4. CoSimulation with Kratos [link](kratos_co_simulation.md)
-->

## Selection of language
The Python interface is recommended for solvers/software-tools implemented in Python. Furthermore it is recommend to be used if the solver/software-tool has an efficient Python interface (efficient in the sense of compute and memory-overhead). The C++ is the most complete one and the C is a practical substitute for solvers/software-tools in other languages avoiding the incompatibilities and complexities of C++ interface.