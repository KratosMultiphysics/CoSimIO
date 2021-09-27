# Info

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Memory management](#memory-management)
- [Setting values](#setting-values)
- [Accessing the Info](#accessing-the-info)
- [Checking if the Info has a certain key](#checking-if-the-info-has-a-certain-key)
- [Checking the size](#checking-the-size)
- [Removing specific keys](#removing-specific-keys)
- [Removing all keys](#removing-all-keys)
- [Using Info in Info](#using-info-in-info)
- [Further information](#further-information)

<!-- /code_chunk_output -->
---

This page describes the C interface of `CoSimIO::Info`. See [here](info_cpp.md) for more information and the native (C++) interface.

## Memory management
The `CoSimIO_Info` object is allocated with `CoSimIO_CreateInfo` and freed with `CoSimIO_FreeInfo`. This is crucial as otherwise memory is leaked!
```c
// create CoSimIO_Info
CoSimIO_Info info = CoSimIO_CreateInfo();

// use info
// ...

// don't forget to free it after using it
CoSimIO_FreeInfo(info);
```
## Setting values
The `Set` method can be used to set values in the `Info`. Note that the type of the value should be specified as template argument. The first argument is the key (given as `std::string`), the second is the value.

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

CoSimIO_FreeInfo(info);
```

One `Info` object can hold several types:
```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);
CoSimIO_Info_SetDouble(info, "tolerance", 0.01);
CoSimIO_Info_SetBool(info, "is_converged", 1);
CoSimIO_Info_SetString(info, "identifier", "fluid");

CoSimIO_FreeInfo(info);
```

Overwritting an existing key with the same or a different data type is possible:
```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);
CoSimIO_Info_SetInt(info, "echo_level", 2);      // this is allowed
CoSimIO_Info_SetDouble(info, "echo_level", 1.5); // this is allowed

CoSimIO_FreeInfo(info);
```

## Accessing the Info
After setting some values in the `CoSimIO_Info`, they can be accessed with the `Get...` functions:

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

int echo_level = CoSimIO_Info_GetInt(info, "echo_level");

CoSimIO_FreeInfo(info);
```

If the wrong function is used an error is thrown at runtime:
```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

double echo_level = CoSimIO_Info_GetDouble(info, "echo_level"); // Error, type mismatch, also tells which type was expected

CoSimIO_FreeInfo(info);
```

## Checking if the Info has a certain key
The `Has` method can be used to check if the `CoSimIO_Info` contains a specific key.

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

int has_echo_level = CoSimIO_Info_Has(info, "echo_level"); // returns 1
int has_tolerance  = CoSimIO_Info_Has(info, "tolerance");  // returns 0

CoSimIO_FreeInfo(info);
```

<!-- ## Getting values with a default
Following the [get method of the Python `dict`](https://docs.python.org/3/library/stdtypes.html#dict.get), the `Get` method can also be used with a default value. If the requested key exists in the `Info`, then the corresponding value is returned. If not, the default is returned.

```c
Info info;

info.Set<int>("echo_level", 1);

int echo_level = info.Get("echo_level", 2); // returns 1 as "echo_level" exists
int verbosity  = info.Get("verbosity", 2);  // returns 2 as "verbosity" doesn't exist
``` -->

## Checking the size
`CoSimIO_Info_Size` can be used get the number of key-value pairs in the `Info`.

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

CoSimIO_Info_Size(info); // returns 1

CoSimIO_Info_SetDouble(info, "tolerance", 0.01);
CoSimIO_Info_SetBool(info, "is_converged", 1);
CoSimIO_Info_SetString(info, "identifier", "fluid");

CoSimIO_Info_Size(info); // returns 4

CoSimIO_FreeInfo(info);
```

## Removing specific keys
The method `CoSimIO_Info_Erase` can be used to remove keys from the `CoSimIO_Info`. Note that it does not throw even if the key doesn't exist.

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

CoSimIO_Info_Erase(info, "echo_level");
CoSimIO_Info_Erase(info, "tolerance"); // does not throw!

CoSimIO_FreeInfo(info);
```

## Removing all keys
The method `CoSimIO_Info_Clear` can be used to remove all keys from the `CoSimIO_Info`.

```c
CoSimIO_Info info = CoSimIO_CreateInfo();

CoSimIO_Info_SetInt(info, "echo_level", 1);

CoSimIO_Info_Clear(info);  // removes everything form info

CoSimIO_FreeInfo(info);
```

<!-- ## Printing the Info
The `CoSimIO::Info` can be printed to a stream:

```c
Info info;

info.Set<int>("echo_level", 1);
info.Set<std::string>("identifier", "fluid");

std::cout << info;

/* This prints to standard output:
CoSimIO-Info; containing 2 entries
  name: echo_level | value: 1 | type: int
  name: identifier | value: fluid | type: string
*/
``` -->

## Using Info in Info
Aside from the basic types (`int`, `double`, `bool`, `std::string`) it is also possible to store an `Info` in an `Info` object. The interface is the same as for the other datatypes:
```c
CoSimIO_Info info_1 = CoSimIO_CreateInfo();
CoSimIO_Info info_2 = CoSimIO_CreateInfo();

CoSimIO_Info_SetInfo(info_1, "sub_info", info_2); // this makes a copy of info_2

CoSimIO_Info retrieved_info = CoSimIO_Info_GetInfo(info, "sub_info"); // this is copied hence needs to be freed

CoSimIO_FreeInfo(info_1);
CoSimIO_FreeInfo(info_2);
CoSimIO_FreeInfo(retrieved_info);

```

This makes it possible to build more complex and hierarchical structures of information. Note that the `Info` is stored as a copy.


## Further information
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/c/co_sim_io_c_info.h) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/tree/master/tests/co_sim_io/c/info).
