# API documentation

This page contains the API documentation of the _CoSimIO_. The public interface is documented in detail, the internal documentation with doxygen is currently work in progress.

## Public interface of _CoSimIO_

### Hello
The `Hello` function can be called without establishing a connection beforehand. It is intended for debugging / setting up the integration of _CoSimIO_. It takes no input and returns an object of `CoSimIO::Info` (see [here](../info/info_cpp.md)), which contains the version info of the _CoSimIO_. It also prints a short hello message.

**Input:**\
None

**Returns:**\
Instance of `CoSimIO::Info` which contains the following:

| name | type | description |
|---|---|---|
| major_version | int    | Major version of _CoSimIO_ |
| minor_version | int    | Minor version of _CoSimIO_ |
| patch_version | string | Patch version of _CoSimIO_ |


### Connect

### Disconnect

### ImportData

### ExportData

### ImportMesh
Uses `CoSimIO::ModelPart` (see [here](../model_part/model_part_cpp.md))

### ExportMesh
Uses `CoSimIO::ModelPart` (see [here](../model_part/model_part_cpp.md))

### Run

### Register

## Doxygen documentation

Work in progress

![](https://media.giphy.com/media/3o7btQ0NH6Kl8CxCfK/giphy.gif)
