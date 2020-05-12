# Tutorials for integrating the _CoSimIO_

This folder contains step-by-step instructions on how to integrate the _CoSimIO_ into a solver. Since the _CoSimIO_ is independent of Kratos, this can be done completely standalone.

Each subfolder has the tutorial for an specific language:
* **CPP Interface**: Is the natural interface of the _CoSimIO_ providing its full capabilities.
* **Python Interface**: Is an extensive interface based on the CPP one which provides the most capabilities.
* **C Interface**: A subset of the CPP interface providing the interface for double and common C arrays with set of functions representing the CPP class methods.

The Python interface is recomended for solvers with python interface. The CPP is the most complete one and the C is a practical substitute for solvers in other languages avoiding the incompatibilities and complexities of CPP interface.

