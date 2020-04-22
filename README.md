~~~
   ______     _____ _           ________
  / ____/___ / ___/(_)___ ___  /  _/ __ |
 / /   / __ \\__ \/ / __ `__ \ / // / / /
/ /___/ /_/ /__/ / / / / / / // // /_/ /
\____/\____/____/_/_/ /_/ /_/___/\____/
Kratos CoSimulationApplication
~~~

# CoSimIO

_CoSimIO_ for exchanging data between different solvers. It works in combination with the [_CoSimulationApplication_](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication)

The implementation is defined in **co_sim_io**\
Examples for coupling solvers in different languages can be found in the **examples** folder\
A tutorial can be found in **tutorial**\
The tests are contained in **tests**\



Mapping (DataTransfer) happens inside of CoSimulation

element types are the same as in vtk

https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf Page 9 & 10

Connect establishes _Connection_ which internally uses _Communication_

print received values / mesh etc in DEBUG like empire did

Precice: Make sending/receiving of multiple things at the same time possible, for improved performance! (writeBlockVectorData)

Future:
- MPI
- Unittests-style for checking functions




maybe add a fct that sends or receives some sort of info?

We are typesafe => no void* => we are using the NATIVE data-formats in each language (Ok fortran not 100% but still ok!)


VIII: 2.9.2: Definition of Coordinate System



Tutorial / UnitTest style

Connect - Disconnect => Hello World


Connect - ImportData - Disconnect
Connect - ExportData - Disconnect
Connect - ImportData - ExportData - Disconnect

Connect - ImportMesh - Disconnect
Connect - ExportMesh - Disconnect
Connect - ImportMesh - ExportMesh - Disconnect

Data Exchange
