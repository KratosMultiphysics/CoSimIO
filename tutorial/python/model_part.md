# ModelPart

The `ModelPart` is a container for mesh, it contains nodes and elements.
It is a simplified version of [`Kratos::ModelPart`](https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h).

### Interface of _CoSimIO.ModelPart_

Create a `ModelPart` like this:
```py
# create CoSimIO.ModelPart
model_part = CoSimIO.ModelPart("my_model_part")

# use model_part
# ...

# access the name of the ModelPart:
name = model_part.Name()
```

Nodes can be created like this:
```python
node = model_part.CreateNewNode(
    1,    # Id
    0.0,  # X-Coordinate
    1.5,  # Y-Coordinate
    -4.22 # Z-Coordinate
)
```

Elements can be created after nodes were created:
```python
connectivity = [1,2] # Ids of the Nodes

element = model_part.CreateNewElement(
    2, # Id
    CoSimIO.ElementType.Line2D2, # Type of element, see "co_sim_io/impl/define.hpp"
    connectivity # Connectivity information, i.e. Ids of nodes that the element has
)
```

Use the following functions to get the number of nodes and elements:
```python
number_of_nodes = model_part.NumberOfNodes()

number_of_elements = model_part.NumberOfElements()
```

The nodes and elements can be iterated with:
```python
# iterate nodes
for node in model_part.Nodes:
    # do sth with node

# iterate elements
for element in model_part.Elements:
    # do sth with element
```

Nodes and elements can also be accessed by Id:
```python
# get the node with Id 3
node = model_part.GetNode(3)

# get the element with Id 12
element = model_part.GetElement(12)
```

Removing all nodes and elements can be done with the following:
```python
# removing all nodes and elements
model_part.Clear()
```

### Interface of _CoSimIO.Node_
The _CoSimIO.Node_ an be used in the following way:
```python
# access Id of node:
node_id = node.Id()

# access the coordinates:
node_x = node.X()
node_y = node.Y()
node_z = node.Z()

coords = node.Coordinates() # [x,y,z]
```

### Interface of _CoSimIO.Element_
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

### Further information
For more information check the [implementation](../../co_sim_io/python/model_part_to_python.hpp) and the [tests](../../tests/co_sim_io/python/test_model_part.py).