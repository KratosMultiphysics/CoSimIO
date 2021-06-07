# Communication

**Input for all communication methods:**

| name | type | required | default| description |
|-------------------|--------|---|---|---|
| my_name           | string | x | - | necessary for establishing a connection |
| connect_to        | string | x | - | name of partner to connect to |
| working_directory | string | - | current working directory | path to the working directory |
| echo_level        | int    | - | 0 | decides how much output is printed |
| print_timing      | bool   | - | false | whether timing information should be printed |


## File-based communication
As the name indicates, this method uses files for communicating data.

**Specific Input:**

| name | type | required | default | description |
|---|---|---|---|---|
| use_folder_for_communication | bool | -  | true  |  whether the files used for communication are written in a dedicated folder. Deadlocks from leftover files from previous executions are less likely to happen as they can be cleanup up.  |

## Socket-based communication
Coming soon ...
