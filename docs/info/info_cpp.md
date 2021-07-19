# Info

The `CoSimIO::Info` object plays a very important role in the _CoSimIO_. It is a map (implemented with `std::map`) holding key-value pairs of different value types, similar to the `dict` of Python. It is used as a configuration object and supports the following value types:
- `int`
- `double`
- `bool`
- `std::string`

Most functions in the public interface of _CoSimIO_ have an instance of `CoSimIO::Info`  as input and as return value to communicate with the including code.

This file describes the native (C++) interface of `CoSimIO::Info`.

Click [here](info_c.md) for the C interface and [here](info_python.md) for the Python interface.


## Setting values
The `Set` method can be used to set values in the `Info`. Note that the type of the value should be specified as template argument. The first argument is the key (given as `std::string`), the second is the value.

~~~c++
Info info;

info.Set<int>("echo_level", 1);
~~~

One `Info` object can hold several types:
~~~c++
Info info;

info.Set<int>("echo_level", 1);
info.Set<double>("tolerance", 0.01);
info.Set<bool>("is_converged" ,true);
info.Set<std::string>("identifier", "fluid");
~~~

Overwritting an existing key with the same or a different data type is possible:
~~~c++
Info info;

info.Set<int>("echo_level", 1);
info.Set<int>("echo_level", 2);      // this is allowed
info.Set<double>("echo_level", 1.5); // this is allowed
~~~

## Accessing the _Info_
After setting some values in the `CoSimIO::Info`, they can be accessed with the `Get` function (again with specifying the template of the value type):

~~~c++
Info info;

info.Set<int>("echo_level", 1);

int echo_level = info.Get<int>("echo_level");
~~~

If the wrong template is used an error is thrown at runtime:
~~~c++
Info info;

info.Set<int>("echo_level", 1);

double echo_level = info.Get<double>("echo_level"); // Error, type mismatch, also tells which type was expected
~~~

## Checking if the _Info_ has a certain key
The `Has` method can be used to check if the `CoSimIO::Info` contains a specific key. Note that this function is not a template.

~~~c++
Info info;

info.Set<int>("echo_level", 1);

bool has_echo_level = info.Has("echo_level"); // returns true
bool has_tolerance  = info.Has("tolerance");  // returns false
~~~

## Getting values with a default
Following the [get method of the Python `dict`](https://docs.python.org/3/library/stdtypes.html#dict.get), the `Get` method can also be used with a default value. If the requested key exists in the `Info`, then the corresponding value is returned. If not, the default is returned.

~~~c++
Info info;

info.Set<int>("echo_level", 1);

int echo_level = info.Get("echo_level", 2); // returns 1 as "echo_level" exists
int verbosity  = info.Get("verbosity", 2);  // returns 2 as "verbosity" doesn't exist
~~~

## Checking the size
`Size` can be used get the number of key-value pairs in the `Info`.

~~~c++
Info info;

info.Set<int>("echo_level", 1);

info.Size(); // returns 1

info.Set<double>("tolerance", 0.01);
info.Set<bool>("is_converged" ,true);
info.Set<std::string>("identifier", "fluid");

info.Size(); // returns 4
~~~

## Removing specific keys
The method `Erase` can be used to remove keys from the `Info`. Note that it does not throw even if the key doesn't exist.

~~~c++
Info info;

info.Set<int>("echo_level", 1);

info.Erase("echo_level");
info.Erase("tolerance"); // does not throw!
~~~

## Removing all keys
The method `Clear` can be used to remove all keys from the `Info`.

~~~c++
Info info;

info.Set<int>("echo_level", 1);

info.Clear(); // removes everything form info
~~~

## Printing the _Info_
The `CoSimIO::Info` can be printed to a stream:

~~~c++
Info info;

info.Set<int>("echo_level", 1);
info.Set<std::string>("identifier", "fluid");

std::cout << info;

/* This prints to standard output:
CoSimIO-Info; containing 2 entries
  name: echo_level | value: 1 | type: int
  name: identifier | value: fluid | type: string
*/
~~~

## Further information (C++ interface)
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/include/info.hpp) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/include/test_info.cpp).
