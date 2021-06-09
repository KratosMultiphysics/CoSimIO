# API documentation

This page contains the API documentation of the _CoSimIO_. The public interface is documented in detail, the internal documentation with doxygen is currently work in progress.

## Public interface of _CoSimIO_

### Hello
The `Hello` function can be called without establishing a connection beforehand. It is intended for debugging / setting up the integration of _CoSimIO_. It takes no input and returns an object of `CoSimIO::Info` (see [here](../info/info_cpp.md)), which contains the version info of the _CoSimIO_. It also prints a short hello message.

#### Input
None

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |
|---|---|---|
| major_version | int    | Major version of _CoSimIO_ |
| minor_version | int    | Minor version of _CoSimIO_ |
| patch_version | string | Patch version of _CoSimIO_ |

* * *


### Connect
This function is used to establish a connection between two partners. It is the first function that must be called, other calls (except `Hello` require an active connection)

#### Requirements
Cannot be called when the same connection was established already.

#### Input
As this function establishes a connection, it also creates the _Communication_ object. Hence it uses the same input as the _Communication_. See [here](../communication.md) for the input.

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |
|---|---|---|
| connection_status | int | status of the connection, can be used to check if connecting was successful

* * *


### Disconnect
Calling this function will disconnect a previously established connection.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name  | string | x | - | output of calling `Connect` |

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |
|---|---|---|
| connection_status | int | status of the connection, can be used to check if disconnecting was successful

* * *


### ImportData
This function is used to import (receive) data from the connection partner. The connection partner has to call `ExportData`.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name  | string | x | - | output of calling `Connect` |
    | identifier       | string | x | - | identifier of the data |
- Vector of data to be exchanged (type depends on language)

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |

* * *


### ExportData
This function is used to export (send) data to the connection partner. The connection partner has to call `ImportData`.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name  | string | x | - | output of calling `Connect` |
    | identifier       | string | x | - | identifier of the data |
- Vector of data to be exchanged (type depends on language)

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |

* * *


### ImportMesh
This function is used to import (receive) a mesh (in the form of a `CoSimIO::ModelPart`) from the connection partner. The connection partner has to call `ExportMesh`.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name  | string | x | - | output of calling `Connect` |
    | identifier       | string | x | - | identifier of the mesh |
- Instance of `CoSimIO::ModelPart`

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |


#### Additional information
See documentation of `CoSimIO::ModelPart` [here](../model_part/model_part_cpp.md).

* * *


### ExportMesh
This function is used to export (send) a mesh (in the form of a `CoSimIO::ModelPart`) to the connection partner. The connection partner has to call `ImportMesh`.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name  | string | x | - | output of calling `Connect` |
    | identifier       | string | x | - | identifier of the mesh |
- Instance of `CoSimIO::ModelPart`

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |


#### Additional information
See documentation of `CoSimIO::ModelPart` [here](../model_part/model_part_cpp.md).

* * *


### Run
The `Run` method used only for remote controlled CoSimulation. Here the _CoSimulationApplication_ takes full control of the solver / code. Functions for controlling the solver / code have to be registered before using the `Register` function.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name          | string | x | - | output of calling `Connect` |

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |

* * *


### Register
This function is used to register the functions that are used to control the solver / code in a remote controlled CoSimulation. It has to be used before calling the `Run` function.

#### Requirements
Can only be called with an active connection (i.e. after calling `Connect` and before calling `Disconnect`).

#### Input
- Instance of `CoSimIO::Info` which contains the following:

    | name | type | required | default| description |
    |-------------------|--------|---|---|---|
    | connection_name          | string | x | - | output of calling `Connect` |
    | function_name            | string | x | - | name of function to be registered (see below) |
- Function pointer to the function that is to be registered

The following functions can be registered:
- AdvanceInTime
- InitializeSolutionStep
- Predict
- SolveSolutionStep
- FinalizeSolutionStep
- OutputSolutionStep
- ImportData
- ExportData
- ImportMesh
- ExportMesh

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |

* * *



## Doxygen documentation

Work in progress

![](https://media.giphy.com/media/3o7btQ0NH6Kl8CxCfK/giphy.gif)
