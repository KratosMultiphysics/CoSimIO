# Tutorial for integrating the _CoSimIO_ using C++ interface

This tutorial helps you through to integrate the _CoSimIO_ into a solver using the C++ interface.

## What you need
- Downloading the _CosimIO_ from the repository:

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
```

- A C++11 compatible compiler. This includes most of the major CPP compilers in Windows, Linux, and Mac. You may find a detailed list of C++11 compatible compilers in https://en.cppreference.com/w/cpp/compiler_support#cpp11


## Tutorial 1: Building
The CPP version of CosimIO is header only and for building you may only include the [co_sim_io.h](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/co_sim_io.hpp) in your project file and compile it with your code:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"

int main(){
    return 0;
}
```

Please don't forget to add the CoSimIO folder to your include path.


## Tutorial 2: Hello CosimIO
After integrating the _CoSimIO_ in your code now it's time to say hello

```c++
auto info = CoSimIO::Hello();
```

Please note that this method like other methods in _CoSimIO_ returns an `Info` object. This object is a versatile container holding important information about the operation that has been done. In this case, it contains the version of the _CoSimIO_ library which can be queried:

```c++
std::cout << info << std::endl;
```

Info class also provides get method for retriving a certain value knowing its type:

```c++
int major_version = info.Get<int>("major_version");
int minor_version = info.Get<int>("minor_version");
std::string patch_version = info.Get<std::string>("patch_version");
```
You may find this example in hello.cpp file in the `solver_integration/cpp` folder


## Tutorial 3: Connecting and Disconnecting
The first step to establishing a connection to Kratos CoSimulation is to use the `Connect()` method:
```c++
// The connect must be called before any CosimIO method called
auto info = CoSimIO::Connect(connection_name, settings);
}
```

First of all, you may notice that `Connect()` method takes an `Info` as its arguments. This container can be used to pass additional information about the solver or connection settings to the CoSimIO:

```c++
CoSimIO::Info settings;
settings.Set("connection_name", "test_connection"); // This must be unique for each connection between two solvers
settings.Set("solver_name", "my_solver"); // Not to be confused with the connection name.
settings.Set("echo_level", 1);
settings.Set("solver_version", "1.25");
}
```
This method returns a `Info` object containing information about the connection which can be queried using Get method:

```c++
int connection_status = info.Get<int>("connection_status");
```

Now putting together everything:

```c++
// CoSimulation includes
#include "co_sim_io.hpp"
int main(){
    CoSimIO::Info settings;
    settings.Set("connection_name", "test_connection"); // This must be unique for each connection between two solvers
    settings.Set("solver_name", "my_solver"); // Not to be confused with the connection name.
    settings.Set("echo_level", 1);
    settings.Set("solver_version", "1.25");

    auto info = CoSimIO::Connect(settings);
    if(info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Connected)
        return 1;
    // Now you may call any CoSimIO methods like ImportData, ExportData, etc.

    // ...
    info = CoSimIO::Disconnect(settings); // disconnect afterwards
    // Here you may use the info but cannot call any CoSimIO method anymore
    if(info.Get<int>("connection_status") != CoSimIO::ConnectionStatus::Disconnected)
        return 1;

    return 0;
}
```

You may find this example in connect_disconect.cpp file in the `solver_integration/cpp` folder

## Tutorial 4: Data Exchange
One of the important missions of the CoSimIO is to send and recieve data between processes. The `ExportData()` method can be used to send data to the Kratos or directly to another solver:

```c++
std::vector<double> data_to_send(4,3.14);
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", "test_connection");
info = CoSimIO::ExportData(info, data_to_send);
```
The `ImportData()` should be used on the other side to recieve data:

```c++
std::vector<double> receive_data;
CoSimIO::Info info;
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", "test_connection");
info = CoSimIO::ImportData(info, receive_data);
```

It is important to mention that the `ImportData()` will clear and resize the vector if needed.


## Tutorial 5: Mesh Exchange
After seeing how we transfer raw data between solvers, it is time to see how we can export and import meshes. For exporting the mesh one may use the `ExportMesh()` method:


```c++
CoSimIO::Info info;
info.Set("identifier", "fluid_mesh");
info.Set("connection_name", "test_connection");
info = CoSimIO::ExportMesh(info,nodal_coordinates, elements_connectivities, elements_types);
```

The arguments are:

* `nodal_coordinates`: A vector of doubles of 3D coordinates of each node in x1,y1,z1,x2,y2,z2,... format:
```c++
std::vector<double> nodal_coordinates{
    0.0, 2.5, 1.0, /*0*/
    2.0, 0.0, 1.5, /*1*/
    2.0, 2.5, 1.5, /*2*/
    4.0, 2.5, 1.7, /*3*/
    4.0, 0.0, 1.7, /*4*/
    6.0, 0.0, 1.8  /*5*/
    };
```
* `elements_connectivities`: A vector of int containing the zero based index of each node in e1_1,e1_2,...,e2_1, e2_2,... format:
```c++
std::vector<int> elements_connectivities = {
    0, 1, 2, /*1*/
    1, 3, 2, /*2*/
    1, 4, 3, /*3*/
    3, 4, 5, /*4*/
};
```

* `elements_types`: A vector of int containing the type of the elements. They are according to the vtk cell types, see [this link](https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf), page 9 & 10.
```c++
std::vector<int> elements_types = {5,5,5,5}; // VTK_TRIANGLE
```

On the other side one can use the `ImportMesh()` method to get the mesh sent by the export:

```c++
info.Clear();
info.Set("identifier", "vector_of_pi");
info.Set("connection_name", "test_connection");

std::vector<double> nodal_coordinates;
std::vector<int> elements_connectivities;
std::vector<int> elements_types;

info = CoSimIO::ImportMesh(info,nodal_coordinates, elements_connectivities, elements_types);
```
