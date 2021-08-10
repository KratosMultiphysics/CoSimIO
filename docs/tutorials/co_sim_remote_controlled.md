# Tutorial for performing coupled simulations with the remote control approach

This tutorial shows how to perform coupled simulations with the Kratos CoSimulationApplication using the remoted control approach.






<!-- This means that the coupling sequence is implemented in the application side and has to match the Kratos side. See [here](kratos_co_simulation.md) for more details on the different approaches.

Note that this page shows the Kratos side, please refer to the corresponding tutorials for the application side, depending on the language.

## Setup

The following setup is used:

- 2 solvers participate in the CoSimulation
- each solver sends one mesh to the CoSimulationApplication at the beginning of the simulation
- each solver has 2 fields that are associated with the mesh, one input and one output.
- the input is imported (i.e. received from the CoSimulationApplication) before solving, the output is exported (i.e. sent to the CoSimulationApplication) after solving

## Input files for Kratos

Kratos requires 2 input files (see the [README](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/README.md#user-guide)):
- `MainKratosCoSim.py`: this file is used to start Kratos.
- `ProjectParametersCoSim.json`: this file contains the settings and the coupling sequence for the CoSimulation. See the [README of the CoSimulationApplication](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/README.md#the-json-configuration-file) for how to write this file and the available options.

Aside from the input files, we need two more files in order to integrate the solver into the CoSimulationApplication:
- `SolverWrapper`: This file is the interface of the solver with the CoSimulationApplication. Examples for wrappers of external solvers can be found. [here](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication/python_scripts/solver_wrappers/external).
- `IO`: The _IO_ is responsible for exchanging data with the solver and the CoSimulationApplication. The _IO_ for using the _CoSimIO_ already exists and can be used directly: [link](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/python_scripts/solver_wrappers/kratos_co_sim_io.py)

### The SolverWrapper

Before setting up the configuration file we need to create the `SolverWrapper` for our solver such that it can be used in the CoSimulationApplication workflow.

Based on our setup from above, we can directly use the [ExternalSolverWrapper.py](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/python_scripts/solver_wrappers/external/external_solver_wrapper.py) which implements the functionalities that we need:
- At the beginning it imports the coupling interfaces/meshes that are exported by the solver
- During the solving phase it first exports the data (which is to be imported by the solver). This is done to synchronize the data in the solver with the data in the CoSimulationApplication, which can be modified by coupling features such as prediction or relaxation. After the solver solved, the CoSimulationApplication then imports the new data such that it can be used in the coupling.

### The ProjectParameters file

The configuration file for a basic FSI example can be found [here](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/README.md#basic-fsi-example). For our example we will arrive to a very similar setup, we only need to modify the `solvers` section in the `solver_settings`.

There we need to use the `SolverWrapper` which we implemented before. In this case the `ExternalSolverWrapper` is being used for both solvers, i.e. the `type` is `"solver_wrappers.external.external_solver_wrapper"`

In the `data` section we have to specify the fields that are being used. E.g. `field_A` and `field_B` as well as the meshes they belong to:

```js
"data" : { // definition of interfaces used in the simulation
    "field_A" : {
        "model_part_name" : "mesh_1",
        "variable_name" : "MESH_DISPLACEMENT"
    },
    "field_B" : {
        "model_part_name" : "mesh_1",
        "variable_name" : "REACTION"
    }
}
```

The `solver_wrapper_settings` contain the following settings:
- "import_meshes"
- "export_data"
- "import_data"

```js
"solver_wrapper_settings" : {
    "import_meshes" : ["mesh_1"],
    "import_data"   : ["field_A"],
    "export_data"   : ["field_B"]
}
```

Each of them has to be set to the data that should be used in the coupling.

Lastly the `io_settings` have to be specified. Since we use the `CoSimIO`, we can use the already existing `kratos_co_sim_io`

```js
"io_settings" : {
    "type" : "kratos_co_sim_io"
}
```

This completes the setup of one of the solvers:

```js
"solver_s" : {
    "data" : {
        "field_A" : {
            "model_part_name" : "mesh_1",
            "variable_name" : "MESH_DISPLACEMENT"
        },
        "field_B" : {
            "model_part_name" : "mesh_1",
            "variable_name" : "REACTION"
        }
    }
    "solver_wrapper_settings" : {
        "import_meshes" : ["mesh_1"],
        "import_data"   : ["field_A"],
        "export_data"   : ["field_B"]
    }
    "io_settings" : {
        "type" : "kratos_co_sim_io"
    }
}
```

The same has to be repeated for the other solver.

Now the Kratos side is ready to perform coupled simulations. -->
