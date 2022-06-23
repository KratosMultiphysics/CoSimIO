# ModelPart

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Interface of CoSimIO::ModelPart](#interface-of-cosimiomodelpart)
  - [Interface for distributed ModelParts (MPI)](#interface-for-distributed-modelparts-mpi)
- [Interface of CoSimIO::Node](#interface-of-cosimionode)
- [Interface of CoSimIO::Element](#interface-of-cosimioelement)
- [Further information](#further-information)

<!-- /code_chunk_output -->
---

The `ModelPart` is a container for mesh, it contains nodes and elements.
It is a simplified version of `Kratos::ModelPart` ([implementation](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h) and [wiki](https://github.com/KratosMultiphysics/Kratos/wiki/ModelPart-and-SubModelPart)).

This page describes the native (C++) interface of `CoSimIO::ModelPart`.

Click [here](model_part_c.md) for the C interface and [here](model_part_python.md) for the Python interface.

## Interface of CoSimIO::ModelPart
Create a _ModelPart_ like this:
```c++
// create CoSimIO::ModelPart
CoSimIO::ModelPart model_part("my_model_part");

// use model_part
// ...

// access the name of the ModelPart:
std::string name = model_part.Name();
```

As single `Node` can be created with `CreateNewNode`:
```c++
CoSimIO::Node& node = model_part.CreateNewNode(
    1,    // Id
    0.0,  // X-Coordinate
    1.5,  // Y-Coordinate
    -4.22 // Z-Coordinate
);
```

Multiple nodes can be created with `CreateNewNodes`:
```c++
std::size_t num_nodes = 10;
std::vector<CoSimIO::IdType> ids(num_nodes);
std::vector<double> x_coords(num_nodes);
std::vector<double> y_coords(num_nodes);
std::vector<double> z_coords(num_nodes);

// initialize the vectors somehow ...

model_part.CreateNewNodes(
    ids,      // Ids
    x_coords, // X-Coordinates
    y_coords, // Y-Coordinates
    z_coords  // Z-Coordinates
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

Multiple elements can be created with `CreateNewElements`:
```c++
std::size_t num_elements = 10;
std::vector<CoSimIO::IdType> ids(num_elements);
std::vector<CoSimIO::ElementType> types(num_elements);
std::vector<CoSimIO::IdType> connectivities;
// the connectivities vector is contiguous, not a vector of vectors!
// Example:
// types is [Line2D2 and Triangle3D3] connectivities is [1,2,3,4,5],
// the {1,2} are the connectivities for the line
// and {3,4,5}  are the connectivities for the triangle

// initialize the vectors somehow ...

model_part.CreateNewElements(
    ids,           // Ids
    types,         // Element types
    connectivities // Connectivities
);
```

Note: Node and Element Ids start with 1 (0 is not accepted).

Use the following functions to get the number of nodes and elements:
```c++
std::size_t number_of_nodes = model_part.NumberOfNodes();

std::size_t number_of_elements = model_part.NumberOfElements();
```

The nodes and elements can be iterated with range-based loops or iterators:
```c++
// iterate nodes (with range based loop)
for (auto& node : model_part.Nodes()) {
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;

}
// iterate nodes (with iterators)
for (auto node_it=model_part.NodesBegin(); node_it!=model_part.NodesEnd(); ++node_it) {
    CoSimIO::Node& node = **node_it;
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;
}

// iterate elements (with range based loop)
for (auto& element : model_part.Elements()) {
    // do sth with element, e.g. print the id:
    std::cout << element.Id() << std::endl;
}

// iterate elements (with iterators)
for (auto elem_it=model_part.ElementsBegin(); elem_it!=model_part.ElementsEnd(); ++elem_it) {
    CoSimIO::Element& element = **elem_it;
    // do sth with element, e.g. print the id:
    std::cout << element.Id() << std::endl;
}
```

Nodes and elements can also be accessed by Id:
```c++
// get the node with Id 3
CoSimIO::Node& node = model_part.GetNode(3);

// get the element with Id 12
CoSimIO::Element& element = model_part.GetElement(12);
```

Iterating is the preferred way of accessing the nodes and elements, access by Id is inherently slower due to the internal data structure of `ModelPart`.

Removing all nodes and elements can be done with the following:
```c++
// removing all nodes and elements
model_part.Clear();
```

### Interface for distributed ModelParts (MPI)
The following interface is provided for `ModelPart`s that are distributed over several processes in MPI-parallel computations.

Ghost nodes that are local in another partition can be created like this:
```c++
CoSimIO::Node& ghost_node = model_part.CreateNewGhostNode(
    1,    // Id
    0.0,  // X-Coordinate
    1.5,  // Y-Coordinate
    -4.2, // Z-Coordinate
    5     // Partition index where the node is local
);
```

Multiple ghost nodes can be created with `CreateNewGhostNodes`, similar to the creation of multiple (local) nodes:
```c++
std::size_t num_nodes = 10;
std::vector<CoSimIO::IdType> ids(num_nodes);
std::vector<double> x_coords(num_nodes);
std::vector<double> y_coords(num_nodes);
std::vector<double> z_coords(num_nodes);
std::vector<int> partition_indices(num_nodes);

// initialize the vectors somehow ...

model_part.CreateNewGhostNodes(
    ids,      // Ids
    x_coords, // X-Coordinates
    y_coords, // Y-Coordinates
    z_coords, // Z-Coordinates
    partition_indices // Partition indices where the nodes are local
);
```

These ghost nodes can also be used for the creation of elements.
Note that this node has to be created as local node in its local partition, otherwise deadlocks can occur!
Also the Ids must be unique, again otherwise deadlocks can occur!

Use the following functions to get the number of local and ghost nodes:
```c++
std::size_t number_of_local_nodes = model_part.NumberOfLocalNodes();

std::size_t number_of_ghost_nodes = model_part.NumberOfGhostNodes();

std::size_t number_of_all_nodes   = model_part.NumberOfNodes(); // local + ghost nodes
```
Note that `model_part.Nodes()` contains all the nodes, i.e. local and ghost nodes.

Iterating through local and ghost nodes is also possible:
```c++
// iterate local nodes
for (auto& local_node : model_part.LocalNodes()) {
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;
}

// iterate ghost nodes
for (auto& ghost_node : model_part.GhostNodes()) {
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;
}
```

## Interface of CoSimIO::Node
The _CoSimIO::Node_ can be used in the following way:
```c++
// access Id of node:
CoSimIO::IdType node_id = node.Id();

// access the coordinates:
double node_x = node.X();
double node_y = node.Y();
double node_z = node.Z();

CoSimIO::CoordinatesType coords = node.Coordinates();
```

## Interface of CoSimIO::Element
The _CoSimIO::Element_ provides the following interface:
```c++
// access Id of element:
CoSimIO::IdType element_id = element.Id();

// the type can be accessed:
CoSimIO::ElementType element_type = element.Type(); // e.g. CoSimIO::ElementType::Point2D or CoSimIO::ElementType::Line2D2

// number of nodes of the element:
std::size_t num_nodes_element = element.NumberOfNodes();

// iterate the nodes of the element (with range based loop):
for (auto& node : element.Nodes()) {
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;
}
// iterate the nodes of the element (with iterators):
for (auto node_it=element.NodesBegin(); node_it!=element.NodesEnd(); ++node_it) {
    CoSimIO::Node& node = **node_it;
    // do sth with node, e.g. print the id:
    std::cout << node.Id() << std::endl;
}
```

## Further information
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/includes/model_part.hpp) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/cpp/test_model_part.cpp).
