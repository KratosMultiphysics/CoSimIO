# ModelPart

[Main Page of Documentation](https://kratosmultiphysics.github.io/CoSimIO/)

**Table of Contents**
<!-- @import "[TOC]" {cmd="toc" depthFrom=2 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Memory management of CoSimIO_ModelPart](#memory-management-of-cosimio_modelpart)
- [Interface of CoSimIO_ModelPart](#interface-of-cosimio_modelpart)
  - [Interface for distributed ModelParts (MPI)](#interface-for-distributed-modelparts-mpi)
- [Interface of CoSimIO_Node](#interface-of-cosimio_node)
- [Interface of CoSimIO_Element](#interface-of-cosimio_element)
- [Further information](#further-information)

<!-- /code_chunk_output -->
---

This page describes the C interface of `CoSimIO::ModelPart`. See [here](model_part_cpp.md) for more information and the native (C++) interface.

## Memory management of CoSimIO_ModelPart
The `CoSimIO_ModelPart` object is allocated with `CoSimIO_CreateModelPart` and freed with `CoSimIO_FreeModelPart`. This is crucial as otherwise memory is leaked!
```c
// create CoSimIO_ModelPart
CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart();

// use model_part
// ...

// don't forget to free it after using it
CoSimIO_FreeModelPart(model_part);
```

Note: The memory of `CoSimIO_Node` and `CoSimIO_Element` is managed by `CoSimIO_ModelPart`, hence no manual management is necessary.

## Interface of CoSimIO_ModelPart
Create and free a `ModelPart` like this:
```c
// create CoSimIO_ModelPart
CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("my_model_part");

// use model_part
// ...

// access the name of the ModelPart:
const char* name = CoSimIO_ModelPart_Name(model_part);

// don't forget to free it after using it
CoSimIO_FreeModelPart(model_part);
```

As single `Node` can be created with `CoSimIO_ModelPart_CreateNewNode`:
```c
/* memory of node is managed by model_part! */
CoSimIO_Node node = CoSimIO_ModelPart_CreateNewNode(
    model_part,
    1,    /* Id */
    0.0,  /* X-Coordinate */
    1.5,  /* Y-Coordinate */
    -4.22 /* Z-Coordinate */
);
```

Multiple nodes can be created with `CoSimIO_ModelPart_CreateNewNodes`:
```c
int num_nodes = 10;
int ids[10];
double x_coords[10];
double y_coords[10];
double z_coords[10];

/* initialize the vectors somehow ... */

CoSimIO_ModelPart_CreateNewNodes(
    model_part,
    num_nodes,
    ids,      /* Ids */
    x_coords, /* X-Coordinates */
    y_coords, /* Y-Coordinates */
    z_coords  /* Z-Coordinates */
);
```

Elements can be created after nodes were created. The mesh connectivites are documented [here](../mesh_connectivities.md).
```c
int connectivity[2] = {1,2}; /* Ids of the Nodes */

/* memory of element is managed by model_part! */
CoSimIO_Element element = CoSimIO_ModelPart_CreateNewElement(
    model_part,
    2, // Id
    CoSimIO_Line2D2, // Type of element
    connectivity // Connectivity information, i.e. Ids of nodes that the element has
);
```

Multiple elements can be created with `CoSimIO_ModelPart_CreateNewElements`:
```c
int num_elements = 10;
int ids[10];
CoSimIO_ElementType types[10];

int num_connectivities = ...; /*to be calculated, depends on element types!*/
int connectivities[...];
/*
the connectivities vector is contiguous, not a vector of vectors!
Example:
types is [Line2D2 and Triangle3D3] connectivities is [1,2,3,4,5],
the {1,2} are the connectivities for the line
and {3,4,5}  are the connectivities for the triangle*/

/* initialize the vectors somehow ... */

CoSimIO_ModelPart_CreateNewElements(
    model_part,
    num_elements,       /* Number of elements */
    ids,                /* Ids */
    types,              /* Element types */
    num_connectivities, /* Number of connectivities */
    connectivities      /* Connectivities */
);
```

Note: Node and Element Ids start with 1 (0 is not accepted).

Use the following functions to get the number of nodes and elements:
```c
int number_of_nodes = CoSimIO_ModelPart_NumberOfNodes(model_part);

int number_of_elements = CoSimIO_ModelPart_NumberOfElements(model_part);
```

The nodes and elements can be iterated with:
```c
/* iterate nodes */
for (int i=0; i<CoSimIO_ModelPart_NumberOfNodes(model_part); ++i) {
    CoSimIO_Node node = CoSimIO_ModelPart_GetNodeByIndex(model_part, i);
    /* do sth with node */
}

// iterate elements
for (int i=0; i<CoSimIO_ModelPart_NumberOfElements(model_part); ++i) {
    CoSimIO_Element element = CoSimIO_ModelPart_GetElementByIndex(model_part, i);
    /* do sth with element */
}
```

Nodes and elements can also be accessed by Id:
```c
// get the node with Id 3
CoSimIO_Node node = CoSimIO_ModelPart_GetNodeById(model_part, 3);

// get the element with Id 12
CoSimIO_Element element = CoSimIO_ModelPart_GetElementById(model_part, 12);
```

Accessing by index is the preferred way of accessing the nodes and elements, access by Id is inherently slower due to the internal data structure of `ModelPart`.

Removing all nodes and elements can be done with the following:
```c
// removing all nodes and elements
CoSimIO_ModelPart_Clear(model_part);
```

### Interface for distributed ModelParts (MPI)
The following interface is provided for `ModelPart`s that are distributed over several processes in MPI-parallel computations.

Ghost nodes that are local in another partition can be created like this:
```c
CoSimIO_Node ghost_node = CoSimIO_ModelPart_CreateNewGhostNode(
    model_part,
    1,    /* Id */
    0.0,  /* X-Coordinate */
    1.5,  /* Y-Coordinate */
    -4.2, /* Z-Coordinate */
    5     /* Partition index where the node is local */
);
```

Multiple ghost nodes can be created with `CreateNewGhostNodes`, similar to the creation of multiple (local) nodes:
```c
int num_nodes = 10;
int ids[10];
double x_coords[10];
double y_coords[10];
double z_coords[10];
int partition_indices[10];

/* initialize the vectors somehow ... */

CoSimIO_ModelPart_CreateNewGhostNodes(
    model_part,
    num_nodes,
    ids,              /* Ids */
    x_coords,         /* X-Coordinates */
    y_coords,         /* Y-Coordinates */
    z_coords,         /* Z-Coordinates */
    partition_indices /* Partition indices where the nodes are local */
);
```

These ghost nodes can also be used for the creation of elements.
Note that this node has to be created as local node in its local partition, otherwise deadlocks can occur!
Also the Ids must be unique, again otherwise deadlocks can occur!

Use the following functions to get the number of local and ghost nodes:
```c
int number_of_local_nodes = CoSimIO_ModelPart_NumberOfLocalNodes(model_part);

int number_of_ghost_nodes = CoSimIO_ModelPart_NumberOfGhostNodes(model_part);

int number_of_all_nodes   = CoSimIO_ModelPart_NumberOfNodes(model_part); // local + ghost nodes
```

Iterating through local and ghost nodes is also possible:
```c
// iterate local nodes
for (int i=0; i<CoSimIO_ModelPart_NumberOfLocalNodes(model_part); ++i) {
    CoSimIO_Node local_node = CoSimIO_ModelPart_GetLocalNodeByIndex(model_part, i);
    // do sth with node
}

// iterate ghost nodes
for (int i=0; i<CoSimIO_ModelPart_NumberOfGhostNodes(model_part); ++i) {
    CoSimIO_Node ghost_node = CoSimIO_ModelPart_GetGhostNodeByIndex(model_part, i);
    // do sth with node
}
```

## Interface of CoSimIO_Node
The _CoSimIO_Node_ can be used in the following way:
```c
// access Id of node:
int node_id = CoSimIO_Node_Id(node);

// access the coordinates:
double node_x = CoSimIO_Node_X(node);
double node_y = CoSimIO_Node_Y(node);
double node_z = CoSimIO_Node_Z(node);

// or with index:
double node_x_idx = CoSimIO_Node_Coordinate(node, 0);
double node_y_idx = CoSimIO_Node_Coordinate(node, 1);
double node_z_idx = CoSimIO_Node_Coordinate(node, 2);
```

## Interface of CoSimIO_Element
The _CoSimIO_Element_ provides the following interface:
```c
// access Id of element:
int element_id = CoSimIO_Element_Id(element);

// the type can be accessed:
CoSimIO_ElementType element_type = CoSimIO_Element_Type(element); // e.g. CoSimIO_Point3D or CoSimIO_Tetrahedra3D4

// number of nodes of the element:
int num_nodes_element = CoSimIO_Element_NumberOfNodes(element);

// iterate the nodes of the element:
for (int i=0; i<CoSimIO_Element_NumberOfNodes(element); ++i) {
    CoSimIO_Node node = CoSimIO_Element_GetNodeByIndex(element, i);
    // do sth with node
}
```

## Further information
For more information check the [implementation](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/c/co_sim_io_c_model_part.h) and the [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/co_sim_io/c/model_part/test_model_part.c).
