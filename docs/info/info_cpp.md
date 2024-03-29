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

The `CoSimIO::Info` object plays a very important role in the _CoSimIO_. It is a map (implemented with `std::map`) holding key-value pairs of different value types, similar to the `dict` of Python. It is used as a configuration object and supports the following value types:
- `int`
- `std::size_t`
- `double`
- `bool`
- `std::string`
- `CoSimIO::Info`

Most functions in the public interface of _CoSimIO_ have an instance of `CoSimIO::Info` as input and as return value to communicate with the including code.

Aside from using the `CoSimIO::Info` as configuration object, it can also be used to exchange metadata between codes by using the `ImportInfo` and `ExportInfo` functions (see the [API documentation](../api_docs/README.md)).

This file describes the native (C++) interface of `CoSimIO::Info`.

Click [here](info_c.md) for the C interface and [here](info_python.md) for the Python interface.


## Setting values
The `Set` method can be used to set values in the `Info`. Note that the type of the value should be specified as template argument. The first argument is the key (given as `std::string`), the second is the value.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);
~~~

One `Info` object can hold several types:
~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);
info.Set<double>("tolerance", 0.01);
info.Set<bool>("is_converged", true);
info.Set<std::string>("identifier", "fluid");
~~~

Overwritting an existing key with the same or a different data type is possible:
~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);
info.Set<int>("echo_level", 2);      // this is allowed
info.Set<double>("echo_level", 1.5); // this is allowed
~~~

## Accessing the Info
After setting some values in the `Info`, they can be accessed with the `Get` function (again with specifying the template of the value type):

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

int echo_level = info.Get<int>("echo_level");
~~~

If the wrong template is used an error is thrown at runtime:
~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

double echo_level = info.Get<double>("echo_level"); // Error, type mismatch, also tells which type was expected
~~~

## Checking if the Info has a certain key
The `Has` method can be used to check if the `Info` contains a specific key. Note that this function is not a template.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

bool has_echo_level = info.Has("echo_level"); // returns true
bool has_tolerance  = info.Has("tolerance");  // returns false
~~~

## Getting values with a default
Following the [get method of the Python `dict`](https://docs.python.org/3/library/stdtypes.html#dict.get), the `Get` method can also be used with a default value. If the requested key exists in the `Info`, then the corresponding value is returned. If not, the default is returned.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

int echo_level = info.Get("echo_level", 2); // returns 1 as "echo_level" exists
int verbosity  = info.Get("verbosity", 2);  // returns 2 as "verbosity" doesn't exist
~~~

## Checking the size
`Size` can be used get the number of key-value pairs in the `Info`.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

info.Size(); // returns 1

info.Set<double>("tolerance", 0.01);
info.Set<bool>("is_converged", true);
info.Set<std::string>("identifier", "fluid");

info.Size(); // returns 4
~~~

## Removing specific keys
The method `Erase` can be used to remove keys from the `Info`. Note that it does not throw even if the key doesn't exist.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

info.Erase("echo_level");
info.Erase("tolerance"); // does not throw!
~~~

## Removing all keys
The method `Clear` can be used to remove all keys from the `Info`.

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);

info.Clear(); // removes everything form info
~~~

## Printing the Info
The `Info` can be printed to a stream:

~~~c++
CoSimIO::Info info;

info.Set<int>("echo_level", 1);
info.Set<std::string>("identifier", "fluid");

std::cout << info;

/* This prints to standard output:
CoSimIO-Info; containing 2 entries
  name: echo_level | value: 1 | type: int
  name: identifier | value: fluid | type: string
*/
~~~

## Using Info in Info
Aside from the basic types (`int`, `double`, `bool`, `std::string`) it is also possible to store an `Info` in an `Info` object. The interface is the same as for the other datatypes:
~~~c++
CoSimIO::Info info_1;
CoSimIO::Info info_2;

info_1.Set<CoSimIO::Info>("info", info_2); // this makes a copy of info_2
~~~

This makes it possible to build more complex and hierarchical structures of information. Note that the `Info` is stored as a copy.


## Further information
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/include/info.hpp) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/include/test_info.cpp).
