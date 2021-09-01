# API documentation

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=3 orderedList=false} -->

<!-- code_chunk_output -->

- [Public interface of _CoSimIO_](#public-interface-of-_cosimio_)
  - [Hello](#hello)
  - [Connect](#connect)
  - [Disconnect](#disconnect)
  - [ImportData](#importdata)
  - [ExportData](#exportdata)
  - [ImportMesh](#importmesh)
  - [ExportMesh](#exportmesh)
  - [Run](#run)
  - [Register](#register)
- [Doxygen documentation](#doxygen-documentation)

<!-- /code_chunk_output -->
---

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::Hello();
~~~

#### Syntax C
~~~c
CoSimIO_Info = CoSimIO_Hello();
~~~


#### Syntax Python
~~~py
info = CoSimIO.Hello()
~~~

* * *


### Connect
This function is used to establish a connection between two partners. It is the first function that must be called, other calls (except `Hello`) require an active connection.

#### Requirements
Cannot be called when the same connection was established already.

#### Input
As this function establishes a connection, it also creates the _Communication_ object. Hence it uses the same input as the _Communication_. See [here](../communication.md) for the input.

#### Returns
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |
|---|---|---|
| connection_name | string | name of connection, necessary for further calls to _CoSimIO_ |
| connection_status | int | status of the connection, can be used to check if connecting was successful |

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::Connect(
    const CoSimIO::Info& I_Settings);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_Connect(
    const CoSimIO_Info I_Settings);
~~~


#### Syntax Python
~~~py
info = CoSimIO.Connect(
    CoSimIO.Info I_Settings)
~~~

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
| connection_status | int | status of the connection, can be used to check if disconnecting was successful |

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::Disconnect(
    const CoSimIO::Info& I_Info);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_Disconnect(
    const CoSimIO_Info I_Info);
~~~


#### Syntax Python
~~~py
info = CoSimIO.Disconnect(
    CoSimIO.Info I_Info)
~~~

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::ImportData(
    const CoSimIO::Info& I_Info,
    std::vector<double>& O_Data);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_ImportData(
    const CoSimIO_Info I_Info,
    int* O_Size,
    double** O_Data);
~~~


#### Syntax Python
~~~py
info = CoSimIO.ImportData(
    CoSimIO.Info I_Info,
    CoSimIO.DoubleVector O_Data)
~~~

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::ExportData(
    const CoSimIO::Info& I_Info,
    const std::vector<double>& I_Data);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_ExportData(
    const CoSimIO_Info I_Info,
    const int I_Size,
    const double* I_Data);
~~~


#### Syntax Python
~~~py
info = CoSimIO.ExportData(
    CoSimIO.Info I_Info,
    CoSimIO.DoubleVector I_Data)
~~~

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::ImportMesh(
    const CoSimIO::Info& I_Info,
    CoSimIO::ModelPart& O_ModelPart);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_ImportMesh(
    const CoSimIO_Info I_Info,
    CoSimIO_ModelPart O_ModelPart);
~~~


#### Syntax Python
~~~py
info = CoSimIO.ImportMesh(
    CoSimIO.Info I_Info,
    CoSimIO.ModelPart O_ModelPart)
~~~


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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::ExportMesh(
    const CoSimIO::Info& I_Info,
    const CoSimIO::ModelPart& I_ModelPart);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_ExportMesh(
    const CoSimIO_Info I_Info,
    const CoSimIO_ModelPart I_ModelPart);
~~~


#### Syntax Python
~~~py
info = CoSimIO.ExportMesh(
    CoSimIO.Info I_Info,
    CoSimIO.ModelPart I_ModelPart)
~~~


#### Additional information
See documentation of `CoSimIO::ModelPart` [here](../model_part/model_part_cpp.md).

* * *


### Run
The `Run` method is only for remote controlled CoSimulation. Here the _CoSimulationApplication_ takes full control of the solver / code. Functions for controlling the solver / code have to be registered before using the `Register` function.

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::Run(
    const CoSimIO::Info& I_Info);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_Run(
    const CoSimIO_Info I_Info);
~~~


#### Syntax Python
~~~py
info = CoSimIO.Run(
    CoSimIO.Info I_Info)
~~~

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

#### Syntax C++
~~~c++
CoSimIO::Info info = CoSimIO::Register(
    const CoSimIO::Info& I_Info,
    std::function<Info(const Info&)> I_FunctionPointer);
~~~

#### Syntax C
~~~c
CoSimIO_Info info = CoSimIO_Register(
    const CoSimIO_Info I_Info,
    CoSimIO_Info (*I_FunctionPointer)(const CoSimIO_Info I_Info));
~~~


#### Syntax Python
~~~py
info = CoSimIO.Register(
    CoSimIO.Info I_Info,
    python_function I_FunctionPointer)
~~~

* * *



## Doxygen documentation

Work in progress

![](https://media.giphy.com/media/3o7btQ0NH6Kl8CxCfK/giphy.gif)
