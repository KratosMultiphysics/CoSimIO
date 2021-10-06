# Building and running Kratos with CoSimulation

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

Before starting the connection to Kratos one should build the Kratos with the necessary applications. Please check the [Kratos install guide](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md) for more information about the Kratos build requirements and options. Here only a brief overview is provided.

The easiest way to build the Kratos would be using the standard_configure files provided in the [scripts](https://github.com/KratosMultiphysics/Kratos/tree/master/scripts) folder. These scripts would provide the Kratos core library but not the required CoSimulation application and (optional but useful) Mapping application. For adding them you may copy for instance the standard_configure.sh to cosimulation_configure.sh and adding these two applications (you may keep or remove the rest):

```bash
# Set applications to compile
export KRATOS_APPLICATIONS=
add_app ${KRATOS_APP_DIR}/CoSimulationApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
```

Now for running the Kratos, one should configure the python environment variables as follows:
* `PYTHONPATH` should point to the Kratos binaries root (`path/to/kratos`) where you can find KratosMultiphysics sub folder
* `LD_LIBRARY_PATH` should point to the libs folder (`path/to/kratos/libs`) where you can find compiled libraries.

After building Kratos it can be run like a regular python script:
```bash
python3 MainKratos.py
```

With `MainKratos.py` typically starting with the imports of Kratos at the top of the script, same procedure as is done with other python modules:

```py
import os # importing a standard python module

import KratosMultiphysics # importing the Kratos Core

# importing the applications
from KratosMultiphysics import CoSimulationApplication
from KratosMultiphysics import MappingApplication
```

## Building and running Kratos with MPI support

For mpi-distributed coupled simulations it is necessary to compile Kratos with MPI support. This can be done by setting the CMake configure flag:
```
-DUSE_MPI=ON
```

See also [the Kratos install guide for MPI support](https://github.com/KratosMultiphysics/Kratos/blob/master/INSTALL.md#parallelism).

Running Kratos in MPI is slightly different from the serial execution. Aside from launching Kratos with `mpiexec` (or `mpirun`) it is important to pass the `--using-mpi` flag:

```bash
mpiexec -np 4 python3 MainKratos.py --using-mpi
```

Now the MPI Core of Kratos (`KratosMultiphysics.mpi`) is available and can be imported and used:
```py
import KratosMultiphysics # importing the Kratos Core
import KratosMultiphysics.mpi as KratosMPI # importing the Kratos MPICore

# importing the applications
from KratosMultiphysics import CoSimulationApplication
from KratosMultiphysics import MappingApplication
```
