# Tutorials for integrating the _CoSimIO_

This folder contains step-by-step instructions on how to integrate the _CoSimIO_ into a solver/software-tool. Since the _CoSimIO_ is independent of Kratos, this can be done completely standalone.

Each subfolder has the tutorial for an specific language:
* **C++ Interface**: Is the natural interface of the _CoSimIO_ providing its full capabilities.
* **Python Interface**: Is an extensive interface based on the C++ one which provides the most capabilities.
* **C Interface**: A subset of the C++ interface providing the interface for double and common C arrays with set of functions representing the C++ class methods.

The Python interface is recommended for solvers/software-tools implemented in Python. Furthermore it is recommend to be used if the solver/software-tool has an efficient Python interface (efficient in the sense of compute and memory-overhead). The C++ is the most complete one and the C is a practical substitute for solvers/software-tools in other languages avoiding the incompatibilities and complexities of C++ interface.

