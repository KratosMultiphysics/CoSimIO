# Tutorial for performing coupled simulations with the Kratos CoSimulationApplication

This tutorial gives an introduction for performing coupled simulations by using the CoSimulationApplication of Kratos. It is strongly recommended to read the [README of the CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/README.md) first.

The _CoSimIO_ can be used in two different ways for conducting coupled simulations.

1. The first approach is the "classical" way of performing coupled simulations. This is usually done when using dedicated coupling tools. Here the code that is to be coupled implements the same coupling sequence as the coupling tool. The advantage is that the sequence of the coupling can be seen, at the cost of lack of flexibility.
The tutorial for this approach can be found [here](co_sim_classical_approach.md).

2. The second approach is called "remote controlled CoSimulation". Here the control of when to execute which part of the coupling sequence is given to the CoSimulationApplication. This has the large advantage that a change in the coupling sequence does not require to change the source code of the including code. This is achieved by registering functions in the _CoSimIO_ that perform the individual tasks in the solving procedure such as solving or data exchange. The disadvantage of this approach is that the including code has the flexibility to create those functions.
The tutorial for this approach can be found [here](co_sim_remote_controlled.md).
