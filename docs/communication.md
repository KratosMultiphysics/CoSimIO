# Communication

Exchanging data between two codes using interprocess communication (IPC) is the main task of the _CoSimIO_. Different methods with specific advantags / disadvantages exist, see below.

When initially establishing a connection between two codes, a handshake of both partners is performed. During this handshake some basic information is exchanged between the partners, including the check if the versions of _CoSimIO_ are compatible.

For the partners to connect without problems, it is recommended to start the connection process close to each other and not with too much time delay.

One connection partner acts as the _primary_ connection, the other one is the _secondary_. This is only required for some technical details, they both have the exact same function and functionalities. Examples of those details:
- Only one partner can create and delete folders to avoid race conditions
- The IPC method used requires different commands from either partner for connecting/disconnecting, e.g. one partner opens a port, the other one connects to it

By default it is automatically determined from the names of the partners which is the _primary_ and which is the _secondary_. In some cases it can be useful to specify this explicitly, this can be done with the setting `is_primary_connection`. Either both partners or none can specify this setting. Note that specifying both partners as either _primary_ or _secondary_ will lead to a deadlock.

The implementation of the different methods for _Communication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/tree/master/co_sim_io/impl/communication).

**Input for all communication methods:**

The following settings are available for all methods of communication:

| name | type | required | default| description |
|-------------------|--------|---|---|---|
| my_name               | string | x | - | necessary for establishing a connection |
| connect_to            | string | x | - | name of partner to connect to |
| is_primary_connection | bool   | - | determined from other input | whether this is the primary connection, if not specified it is determined automatically from the names of the partners |
| working_directory     | string | - | current working directory | path to the working directory |
| echo_level            | int    | - | 0 | decides how much output is printed |
| print_timing          | bool   | - | false | whether timing information should be printed |


## File-based communication
As the name indicates, this method uses files for communicating data. It is robust and useful for debugging. It is the preferred method for implementing the _CoSimIO_ in a new code, as it is intuitive to follow the flow of data through the files.

It is important to mention that leftover files (e.g. when one side crashes) can interfere with following executions, as these leftover files can be mistakenly read. A remedy (but not a solution) is to use a dedicated folder where the files are written (this is the default behavior and can be changed with the setting `use_folder_for_communication`, see below). The folder is created (or deleted and recreated if it exists from a previous execution) at the beginning of the execution by the _primary_ connection, the files for the data exchange are then written inside this folder. At the end of the execution it is automatically deleted. This way it is less likely that leftover files from previous executions interfere.\
Note however that also in this case it is possible that a leftover folder can cause issues. Two safe methods exist to avoid this, but they require more input / actions from the user:

- Make sure the communication folder / communication files from previous executions are deleted before starting again
- explicitly specify which partner is the _primary_ and which is the _secondary_ connection. Then start the _primary_ with a delay before the _secondary_, so that it has time to remove potential leftovers.

The implementation of the _FileCommunication_ can be found [here](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/impl/communication/file_communication.hpp).

**Specific Input:**

| name | type | required | default | description |
|---|---|---|---|---|
| use_folder_for_communication | bool | -  | true  | whether the files used for communication are written in a dedicated folder. Deadlocks from leftover files from previous executions are less likely to happen as they can be cleanup up. |

## Socket-based communication
Coming soon ...
