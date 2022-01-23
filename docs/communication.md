# Communication

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [File-based communication](#file-based-communication)
- [Socket-based communication](#socket-based-communication)
- [Unix domain socket-based communication](#unix-domain-socket-based-communication)
- [Pipe-based communication](#pipe-based-communication)

<!-- /code_chunk_output -->
---

Exchanging data between two codes using interprocess communication (IPC) is the main task of the _CoSimIO_. Different methods with specific advantags / disadvantages exist, see below.

When initially establishing a connection between two codes, a handshake of both partners is performed. During this handshake some basic information is exchanged between the partners, including the check if the versions of _CoSimIO_ are compatible. The handshake is always done via files as it is the most robust way of communication.

For the partners to connect without problems, it is recommended to start the connection process close to each other and not with too much time delay.

One connection partner acts as the _primary_ connection, the other one is the _secondary_. This is only required for some technical details, they both have the exact same function and functionalities. Examples of those details:
- Only one partner can create and delete folders to avoid race conditions
- The IPC method used requires different commands from either partner for connecting/disconnecting, e.g. one partner opens a port, the other one connects to it

By default it is automatically determined from the names of the partners which is the _primary_ and which is the _secondary_. In some cases it can be useful to specify this explicitly, this can be done with the setting `is_primary_connection`. Either both partners or none can specify this setting. Note that specifying both partners as either _primary_ or _secondary_ will lead to a deadlock.

It is important to mention that leftover files (e.g. when one side crashes) can interfere with following executions, as these leftover files can be mistakenly read. A remedy (but not a solution) is to use a dedicated folder where the files are written (this is the default behavior and can be changed with the setting `use_folder_for_communication`, see below). The folder is created (or deleted and recreated if it exists from a previous execution) at the beginning of the execution by the _primary_ connection, the files for the data exchange are then written inside this folder. At the end of the execution it is automatically deleted. This way it is less likely that leftover files from previous executions interfere.\
Note however that also in this case it is possible that a leftover folder can cause issues. Two safe methods exist to avoid this, but they require more input / actions from the user:

- Make sure the communication folder / communication files from previous executions are deleted before starting again
- explicitly specify which partner is the _primary_ and which is the _secondary_ connection. Then start the _primary_ with a delay before the _secondary_, so that it has time to remove potential leftovers.

The implementation of the different methods for _Communication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/tree/master/co_sim_io/includes/communication).

**Input for all communication methods:**

The following settings are available for all methods of communication:

| name | type | required | default| description |
|-------------------|--------|---|---|---|
| my_name               | string | x | - | necessary for establishing a connection |
| connect_to            | string | x | - | name of partner to connect to |
| communication_format  | string | - | file | select format of communication, see below |
| is_primary_connection | bool   | - | determined from other input | whether this is the primary connection, if not specified it is determined automatically from the names of the partners |
| working_directory     | string | - | current working directory | path to the working directory |
| use_folder_for_communication | bool | - | true  | whether the files used for communication are written in a dedicated folder. Deadlocks from leftover files from previous executions are less likely to happen as they can be cleanup up. |
| always_use_serializer | bool | - | false  | use the Serializer also when it is not necessary, e.g. for basic types such as Im-/ExportData. This is ~ 10x slower but more stable, especially when combined with ascii-serialization |
| serializer_trace_type | string | - | no_trace | mode for the `Serializer`: `no_trace` (fastest method, binary format, without any debugging checks), `ascii` (ascii format, without any debugging checks), `trace_error` (ascii format, checks are enabled), `trace_all` (ascii format, checks are enabled and printed, hence very verbose!) |
| echo_level            | int    | - | 0 | decides how much output is printed |
| print_timing          | bool   | - | false | whether timing information should be printed |

<!-- ## Comparison of communication methods

| Communication | Support for distributed environments (MPI) | Support for communication across operating systems |
|---|---|---|
|File| yes | yes |
|Pipe| yes | no |
|Sockets| yes | yes | -->



## File-based communication
As the name indicates, this method uses files for communicating data. It is robust and useful for debugging. It is the preferred method for implementing the _CoSimIO_ in a new code, as it is intuitive to follow the flow of data through the files.
In order to prevent race conditions when accessing the files, two mechanisms are available (which can be selected with `use_aux_file_for_file_availability`):
- `use_aux_file_for_file_availability == true`: An empty auxiliary file is created once the real file with the data is ready to be read. This is used unconditionally for any synchronization files.
- `use_aux_file_for_file_availability == false`: On Unix operating systems, renaming of files is atomic (on Windows it is not which can cause race conditions!). Hence the data is written to a file with a temporary name. Once the writing is complete, the file is renamed to the real name for the other partner to read from it. This is especially useful for clusters since it avoids writing of auxiliar files on slow filesystems.

The implementation of the _FileCommunication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/includes/communication/file_communication.hpp).

**Specific Input:**

Set `communication_format` to `file`.

| name | type | required | default| description |
|---|---|---|---|---|
| use_aux_file_for_file_availability | bool | - | Windows: true; Unix: false  | select whether files are made available by use of an auxiliary file or via rename. |
| use_file_serializer | bool   | - | true | Using the `FileSerializer` (which directly uses a file stream to read/write data) over the `StreamSerializer` (which first to reads/writes to a stringstream before writing everything to the file at once) |

## Socket-based communication
The data is communicated through network sockets by using the TCP communication protocol (using IPv4). No data is written to the filesystem, this makes it more efficient than the file-based communication.
**Important**: By default the loopback interface / localhost is used, which cannot communicate across compute nodes. Hence for using this form of communication on a cluster or other system with multiple compute nodes, it is required to specify which network should be used (e.g. infiniband), see below how to specify the input. Otherwise it will hang! On most linux systems the `ifconfig` command can be used to check the available networks. Otherwise contact your system administrator.

The [ASIO](https://think-async.com/Asio/) library is used as a high level interface for the sockets.

The implementation of the _SocketCommunication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/includes/communication/socket_communication.hpp).

**Specific Input:**

Set `communication_format` to `socket`.

| name | type | required | default| description |
|---|---|---|---|---|
| ip_address | string | - | "127.0.0.1" | specify the ip address used to establish the connection |
| network_name | string | - | - | the name of the network can be specified _alternatively_ to specifying the ip address. This is used to determine the ip address. Will print the available networks if a wrong name is specified. |

The following logic is used for selecting the ip-address
1. If the user has specified `ip_address`, then this one is used directly
2. If instead the `network_name` is specified, then this is used to determine the ip-address to be used
3. If neither is specified, then the localhost ip-address is used (`127.0.0.1`)

## Unix domain socket-based communication
**This form of communication is experimental**
This type of communication uses unix domain sockets for the data exchange. It is similar to the tcp socket communication, but instead of passing the data through the network, it uses the kernel memory. This makes it faster, but at the same time it only works locally on one compute node.

The [ASIO](https://think-async.com/Asio/) library is used as a high level interface for the sockets.

This form of communication is currently only available under Unix, the Windows implementation is work in progress.

The implementation of the _LocalSocketCommunication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/includes/communication/local_socket_communication.hpp).

**Specific Input:**

Set `communication_format` to `local_socket`.

| name | type | required | default| description |
|---|---|---|---|---|
|currently_nothing|

## Pipe-based communication
**This form of communication is experimental**
A pipe is a data channel to perform interprocess communication between two processes. No data is written to the filesystem, it is directly exchanged through the kernel memory. This makes it more efficient than the file-based communication, but at the same time it only works locally on one compute node.

The (default) buffer size is specific for each operating system, see e.g. [here](https://man7.org/linux/man-pages/man7/pipe.7.html) for information for Linux. If the data to be exchanged is larger than the buffer size, then it is exchanged in chuncks. Hence a larger buffer size results in less data exchanges.

This form of communication is currently only available under Unix, the Windows implementation is work in progress.

The implementation of the _PipeCommunication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/includes/communication/pipe_communication.hpp).

**Specific Input:**

Set `communication_format` to `pipe`.

| name | type | required | default| description |
|---|---|---|---|---|
| buffer_size | int | - | 8192 (8 KB) | buffer size of pipe, differs between OSs. |
