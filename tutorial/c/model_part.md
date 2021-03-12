# ModelPart

The `ModelPart` is a container for mesh, it contains nodes and elements.
It is a simplified version of [`Kratos::ModelPart`](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h).

### Interface of _CoSimIO_ModelPart_
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

Nodes can be created like this:
```c
// memory of node is managed by model_part!
CoSimIO_Node node = CoSimIO_ModelPart_CreateNewNode(
    model_part,
    1,    // Id
    0.0,  // X-Coordinate
    1.5,  // Y-Coordinate
    -4.22 // Z-Coordinate
);
```

Elements can be created after nodes were created:
```c
int connectivity[2] = {1,2}; // Ids of the Nodes

// memory of element is managed by model_part!
CoSimIO_Element element = CoSimIO_ModelPart_CreateNewElement(
    model_part,
    2, // Id
    CoSimIO_Line2D2, // Type of element, see "co_sim_io/c/co_sim_io_c_model_part.h"
    connectivity // Connectivity information, i.e. Ids of nodes that the element has
);
```

Use the following functions to get the number of nodes and elements:
```c
int number_of_nodes = CoSimIO_ModelPart_NumberOfNodes(model_part);

int number_of_elements = CoSimIO_ModelPart_NumberOfElements(model_part);
```

The nodes and elements can be iterated with:
```c
// iterate nodes
for (int i=0; i<CoSimIO_ModelPart_NumberOfNodes(model_part); ++i) {
    CoSimIO_Node node = CoSimIO_ModelPart_GetNodeByIndex(model_part, i);
    // do sth with node
}

// iterate elements
for (int i=0; i<CoSimIO_ModelPart_NumberOfElements(model_part); ++i) {
    CoSimIO_Element element = CoSimIO_ModelPart_GetElementByIndex(model_part, i);
    // do sth with element
}
```

Nodes and elements can also be accessed by Id:
```c
// get the node with Id 3
CoSimIO_Node node = CoSimIO_ModelPart_GetNodeById(model_part, 3);

// get the element with Id 12
CoSimIO_Element element = CoSimIO_ModelPart_GetElementById(model_part, 12);
```

Removing all nodes and elements can be done with the following:
```c
// removing all nodes and elements
CoSimIO_ModelPart_Clear(model_part);
```

### Interface of _CoSimIO_Node_
The _CoSimIO_Node_ an be used in the following way:
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

### Interface of _CoSimIO_Element_
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

### Further information
For more information check the [implementation](../../co_sim_io/c/co_sim_io_c_model_part.h) and the [tests](../../tests/co_sim_io/c/model_part/test_model_part.c).