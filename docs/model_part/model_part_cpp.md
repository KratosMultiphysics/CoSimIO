# ModelPart

The `ModelPart` is a container for mesh, it contains nodes and elements.
It is a simplified version of `Kratos::ModelPart` ([link](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h)).

This file describes the native (C++) interface of `Kratos::ModelPart`.

Click [here](model_part_c.md) for the C interface and [here](model_part_python.md) for the Python interface.

## C++ interface of _CoSimIO::ModelPart_
Create a `ModelPart` like this:
```c++
// create CoSimIO::ModelPart
CoSimIO::ModelPart model_part("my_model_part");

// use model_part
// ...

// access the name of the ModelPart:
std::string name = model_part.Name();
```

Nodes can be created like this:
```c++
CoSimIO::Node& node = model_part.CreateNewNode(
    1,    // Id
    0.0,  // X-Coordinate
    1.5,  // Y-Coordinate
    -4.22 // Z-Coordinate
);
```

Elements can be created after nodes were created. The mesh connectivites are documented [here](../mesh_connectivities.md).
```c++
std::vector<CoSimIO::IdType> connectivity {1,2}; // Ids of the Nodes

CoSimIO::Element& element = model_part.CreateNewElement(
    2, // Id
    CoSimIO::ElementType::Line2D2, // Type of element
    connectivity // Connectivity information, i.e. Ids of nodes that the element has
);
```

Use the following functions to get the number of nodes and elements:
```c++
std::size_t number_of_nodes = model_part.NumberOfNodes();

std::size_t number_of_elements = model_part.NumberOfElements();
```

The nodes and elements can be iterated with:
```c++
// iterate nodes
for (auto node_it=model_part.NodesBegin(); node_it!=model_part.NodesEnd(); ++node_it) {
    CoSimIO::Node& node = **node_it;
    // do sth with node
}

// iterate elements
for (auto elem_it=model_part.ElementsBegin(); elem_it!=model_part.ElementsEnd(); ++elem_it) {
    CoSimIO::Element& element = **elem_it;
    // do sth with element
}
```

Nodes and elements can also be accessed by Id:
```c++
// get the node with Id 3
CoSimIO::Node& node = model_part.GetNode(3);

// get the element with Id 12
CoSimIO::Element& element = model_part.GetElement(12);
```

Removing all nodes and elements can be done with the following:
```c++
// removing all nodes and elements
model_part.Clear();
```

## C++ interface of _CoSimIO::Node_
The _CoSimIO::Node_ an be used in the following way:
```c++
// access Id of node:
CoSimIO::IdType node_id = node.Id();

// access the coordinates:
double node_x = node.X();
double node_y = node.Y();
double node_z = node.Z();

CoSimIO::CoordinatesType coords = node.Coordinates();
```

## C++ interface of _CoSimIO::Element_
The _CoSimIO::Element_ provides the following interface:
```c++
// access Id of element:
CoSimIO::IdType element_id = element.Id();

// the type can be accessed:
CoSimIO::ElementType element_type = element.Type(); // e.g. CoSimIO::ElementType::Point2D or CoSimIO::ElementType::Line2D2

// number of nodes of the element:
std::size_t num_nodes_element = element.NumberOfNodes();

// iterate the nodes of the element:
for (auto node_it=element.NodesBegin(); node_it!=element.NodesEnd(); ++node_it) {
    CoSimIO::Node& node = **node_it;
    // do sth with node
}
```

## Further information (C++ interface)
For more information check the [implementation](../../co_sim_io/impl/model_part.hpp) and the [tests](../../tests/co_sim_io/impl/test_model_part.cpp).
