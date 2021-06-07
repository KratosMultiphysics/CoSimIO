# Info

The `CoSimIO::Info` object plays a very important role in the _CoSimIO_. It is used as a configuration object that can hold different types of data, similar to the `dict` of Python. The following types are suppoerted:
- `int`
- `double`
- `bool`
- `std::string`

Most functions in the public interface of _CoSimIO_ have an instance of `CoSimIO::Info`  as input and as return value to communicate with the including code.

This file describes the native (C++) interface of `CoSimIO::Info`.

Click [here](info_c.md) for the C interface and [here](info_python.md) for the Python interface.
