# Tutorial for integrating the _CoSimIO_

This folder contains step-by-step instructions on how to integrate the _CoSimIO_ into a solver. Since the _CoSimIO_ is independent of Kratos, this can be done completely standalone.

It is recommended to first take a look at the provided [examples](../examples) to get a basic idea of how the integration of the CoSimIO works.






After the integration is done


Which type of CoSimulation is desired?

The _CoSimIO_ offers basically two ways of performing CoSimulation:
- "Classical" way of CoSimulation:


- Giving full control to Kratos & CoSimulationApplication

1. Include the neccessary include for the respective language
2. Add calls to Connect and Disconnect
3. - Classical way:
      1. integrate the necessary function calls
   - Fully coupled CoSim
     1. Create the functions needed by your solver
     2. Register them in _CoSimIO_
     3. Add the call for _CoSimIO::Run_

- Example drivers (maybe change those to Unittests? => probably not if this works well)
  - Connect - Disconnect
  - etc

- Run a full example with Kratos


- MPCCI S.698