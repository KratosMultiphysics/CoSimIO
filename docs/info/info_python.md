# Info

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Setting values](#setting-values)
- [Accessing the Info](#accessing-the-info)
- [Checking if the Info has a certain key](#checking-if-the-info-has-a-certain-key)
- [Getting values with a default](#getting-values-with-a-default)
- [Checking the size](#checking-the-size)
- [Removing specific keys](#removing-specific-keys)
- [Removing all keys](#removing-all-keys)
- [Printing the Info](#printing-the-info)
- [Using Info in Info](#using-info-in-info)
- [Further information](#further-information)

<!-- /code_chunk_output -->
---

This page describes the Python interface of `CoSimIO::Info`. See [here](info_cpp.md) for more information and the native (C++) interface.

## Setting values

The `Set...` methods can be used to set values in the `Info`. The first argument is the key (given as `string`), the second is the value.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)
```

One `Info` object can hold several types:

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)
info.SetDouble("tolerance", 0.01)
info.SetBool("is_converged", True)
info.SetString("identifier", "fluid")
```

Overwritting an existing key with the same or a different data type is possible:

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)
info.SetInt("echo_level", 2)      # this is allowed
info.SetDouble("echo_level", 1.5) # this is allowed
```

## Accessing the Info

After setting some values in the `Info`, they can be accessed with the `Get...` functions:

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

echo_level = info.GetInt("echo_level")
```

If the wrong function is used an error is thrown at runtime:

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

echo_level = info.GetDouble("echo_level") # Error, type mismatch, also tells which type was expected
```

## Checking if the Info has a certain key

The `Has` method can be used to check if the `Info` contains a specific key. Note that this function is not a template.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

has_echo_level = info.Has("echo_level") # returns True
has_tolerance  = info.Has("tolerance")  # returns False
```

## Getting values with a default

Following the [get method of the Python `dict`](https://docs.python.org/3/library/stdtypes.html#dict.get), the `Get...` methods can also be used with a default value. If the requested key exists in the `Info`, then the corresponding value is returned. If not, the default is returned.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

echo_level = info.Get("echo_level", 2) # returns 1 as "echo_level" exists
verbosity  = info.Get("verbosity",  2) # returns 2 as "verbosity" doesn't exist
```

## Checking the size
`Size` can be used get the number of key-value pairs in the `Info`.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

info.Size() # returns 1

info.SetDouble("tolerance", 0.01)
info.SetBool("is_converged", True)
info.SetString("identifier", "fluid")

info.Size() # returns 4
```

## Removing specific keys

The method `Erase` can be used to remove keys from the `Info`. Note that it does not throw even if the key doesn't exist.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

info.Erase("echo_level")
info.Erase("tolerance") # does not throw!
```

## Removing all keys

The method `Clear` can be used to remove all keys from the `Info`.

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)

info.Clear() # removes everything form info
```

## Printing the Info

The `Info` can be printed:

```py
info = CoSimIO.Info()

info.SetInt("echo_level", 1)
info.SetString("identifier", "fluid")

print(info)

''' This prints to standard output:
CoSimIO-Info; containing 2 entries
  name: echo_level | value: 1 | type: int
  name: identifier | value: fluid | type: string
'''
```

## Using Info in Info

Aside from the basic types (`int`, `double`, `bool`, `string`) it is also possible to store an `Info` in an `Info` object. The interface is the same as for the other datatypes:

```py
Info info_1
Info info_2

info_1.SetInfo("info", info_2) # this makes a copy of info_2
```

This makes it possible to build more complex and hierarchical structures of information. Note that the `Info` is stored as a copy.

## Further information

For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/python/info_to_python.hpp) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/python/test_info.py).
