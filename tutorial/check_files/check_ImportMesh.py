'''
This file is intended to check the "ImportMesh" function of a Solver using CoSimIO
Requirements: compiling the Python-interface of CoSimIO (see the docs)

Instructions:
    1. The solver that is to be checked connects to CoSimIO with the connection-name "check_ImportMesh"
    2. The solver calls "ImportMesh" with identifier "mesh_to_exchange"
    3. Run this script in a separate terminal. If it runs through then everything works as expected
    4. Check the imported mesh. It should be the same as the mesh that is defined below

For a python-solver this could be the code to check the ImportMesh function:

import CoSimIO

connection_name = "check_ImportMesh"
identifier = "mesh_to_exchange"

settings = {} # using default settings
CoSimIO.Connect(connection_name, settings)

# checking if the ImportMesh function works as expected
nodal_coords, element_connectivities, element_types = CoSimIO.ImportMesh(connection_name, identifier)

# now checking if the expected values match the imported ones ...

CoSimIO.Disconnect(connection_name)
'''

import CoSimIO

connection_name = "check_ImportMesh"
identifier = "mesh_to_exchange"

'''
  -- Mesh --
0      2      3
x------x------x
 \     |     /|\
  \  1 |  2 / | \
   \   |   /  |  \
    \  |  /   |   \
     \ | /  3 |  4 \
      \|/     |     \
       x------x-----x
       1      4     5
'''

nodal_coords = [
    0.0, 2.5, 1.0, # 0
    2.0, 0.0, 1.5, # 1
    2.0, 2.5, 1.5, # 2
    4.0, 2.5, 1.7, # 3
    4.0, 0.0, 1.7, # 4
    6.0, 0.0, 1.8  # 5
]

element_connectivities = [
    0, 1, 2, # 1
    1, 3, 2, # 2
    1, 4, 3, # 3
    3, 4, 5, # 4
];

element_types = [
    5,5,5,5 # VTK_TRIANGLE
]
settings = {"is_connection_master" : "1"} # "is_connection_master" should only be used by Kratos!
CoSimIO.Connect(connection_name, settings)

# the solver to be checked calls "ImportMesh", hence here we call the counterpart "ExportMesh"
CoSimIO.ExportMesh(connection_name, "mesh_to_exchange", nodal_coords, element_connectivities, element_types)
print("Mesh was exported")

CoSimIO.Disconnect(connection_name)

print("Exporting Mesh Successful!")
