# ModelPart

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Interface of CoSimIO.ModelPart](#interface-of-cosimiomodelpart)
  - [Interface for distributed ModelParts (MPI)](#interface-for-distributed-modelparts-mpi)
- [Interface of CoSimIO.Node](#interface-of-cosimionode)
- [Interface of CoSimIO.Element](#interface-of-cosimioelement)
- [Further information](#further-information)

<!-- /code_chunk_output -->
---

This page describes the Python interface of `CoSimIO::ModelPart`. See [here](model_part_cpp.md) for more information and the native (C++) interface.

## Interface of CoSimIO.ModelPart

Create a `ModelPart` like this:
```py
# create CoSimIO.ModelPart
model_part = CoSimIO.ModelPart("my_model_part")

# use model_part
# ...

# access the name of the ModelPart:
name = model_part.Name()
```

As single `Node` can be created with `CreateNewNode`:
```python
node = model_part.CreateNewNode(
    1,    # Id
    0.0,  # X-Coordinate
    1.5,  # Y-Coordinate
    -4.22 # Z-Coordinate
)
```

Multiple nodes can be created with `CreateNewNodes`:
```python
num_nodes = 10
ids = [...] # len = num_nodes
x_coords = [...] # len = num_nodes
y_coords = [...] # len = num_nodes
z_coords = [...] # len = num_nodes

model_part.CreateNewNodes(
    ids,      # Ids
    x_coords, # X-Coordinates
    y_coords, # Y-Coordinates
    z_coords  # Z-Coordinates
)
```

Elements can be created after nodes were created. The mesh connectivites are documented [here](../mesh_connectivities.md).
```python
connectivity = [1,2] # Ids of the Nodes

element = model_part.CreateNewElement(
    2, # Id
    CoSimIO.ElementType.Line2D2, # Type of element
    connectivity # Connectivity information, i.e. Ids of nodes that the element has
)
```

Multiple elements can be created with `CreateNewElements`:
```python
std::size_t num_elements = 10
ids = [...] # len = num_elements
types = [...] # len = num_elements
connectivities = [...] # len depends on element types
# the connectivities list is contiguous, not a list of list!
# Example:
# types is [Line2D2 and Triangle3D3] connectivities is [1,2,3,4,5],
# the [1,2] are the connectivities for the line
# and [3,4,5]  are the connectivities for the triangle

model_part.CreateNewElements(
    ids,           # Ids
    types,         # Element types
    connectivities # Connectivities
)
```

Note: Node and Element Ids start with 1 (0 is not accepted).

Use the following functions to get the number of nodes and elements:
```python
number_of_nodes = model_part.NumberOfNodes()

number_of_elements = model_part.NumberOfElements()
```

The nodes and elements can be iterated with:
```python
# iterate nodes
for node in model_part.Nodes:
    # do sth with node, e.g. print the id:
    print(node.Id())

# iterate elements
for element in model_part.Elements:
    # do sth with element, e.g. print the id:
    print(element.Id())
```

Nodes and elements can also be accessed by Id:
```python
# get the node with Id 3
node = model_part.GetNode(3)

# get the element with Id 12
element = model_part.GetElement(12)
```

Iterating is the preferred way of accessing the nodes and elements, access by Id is inherently slower due to the internal data structure of `ModelPart`.

Removing all nodes and elements can be done with the following:
```python
# removing all nodes and elements
model_part.Clear()
```

### Interface for distributed ModelParts (MPI)
The following interface is provided for `ModelPart`s that are distributed over several processes in MPI-parallel computations.

Ghost nodes that are local in another partition can be created like this:
```python
ghost_node = model_part.CreateNewGhostNode(
    1,    # Id
    0.0,  # X-Coordinate
    1.5,  # Y-Coordinate
    -4.2, # Z-Coordinate
    5     # Partition index where the node is local
)
```

Multiple ghost nodes can be created with `CreateNewGhostNodes`, similar to the creation of multiple (local) nodes:
```python
num_ghost_nodes = 10
ids = [...] # len = num_ghost_nodes
x_coords = [...] # len = num_ghost_nodes
y_coords = [...] # len = num_ghost_nodes
z_coords = [...] # len = num_ghost_nodes
partition_indices = [...] # len = num_ghost_nodes

model_part.CreateNewGhostNodes(
    ids,      # Ids
    x_coords, # X-Coordinates
    y_coords, # Y-Coordinates
    z_coords, # Z-Coordinates
    partition_indices # Partition indices where the nodes are local
```

These ghost nodes can also be used for the creation of elements.
Note that this node has to be created as local node in its local partition, otherwise deadlocks can occur!
Also the Ids must be unique, again otherwise deadlocks can occur!

Use the following functions to get the number of local and ghost nodes:
```python
number_of_local_nodes = model_part.NumberOfLocalNodes()

number_of_ghost_nodes = model_part.NumberOfGhostNodes()

number_of_all_nodes   = model_part.NumberOfNodes() # local + ghost nodes
```
Note that `model_part.Nodes()` contains all the nodes, i.e. local and ghost nodes.

Iterating through local and ghost nodes is also possible:
```python
# iterate local nodes
for local_node in model_part.LocalNodes:
    # do sth with node, e.g. print the id:
    print(local_node.Id())

# iterate ghost nodes
for ghost_node in model_part.GhostNodes:
    # do sth with node, e.g. print the id:
    print(ghost_node.Id())
```

## Interface of CoSimIO.Node
The _CoSimIO.Node_ can be used in the following way:
```python
# access Id of node:
node_id = node.Id()

# access the coordinates:
node_x = node.X()
node_y = node.Y()
node_z = node.Z()

coords = node.Coordinates() # [x,y,z]
```

## Interface of CoSimIO.Element
The _CoSimIO.Element_ provides the following interface:
```python
# access Id of element:
element_id = element.Id()

# the type can be accessed:
element_type = element.Type() # e.g. CoSimIO.ElementType.Point2D or CoSimIO.ElementType.Line2D2

# number of nodes of the element:
num_nodes_element = element.NumberOfNodes()

# iterate the nodes of the element:
for node in element.Nodes:
    # do sth with node
```

## Further information
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/python/model_part_to_python.hpp) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/python/test_model_part.py).
