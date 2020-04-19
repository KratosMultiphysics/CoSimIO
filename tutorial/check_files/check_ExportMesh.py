'''
This file is intended to check the "ExportMesh" function of a Solver using CoSimIO
Requirements: compiling the Python-interface of CoSimIO (see the docs)

Instructions:
    1. The solver that is to be checked connects to CoSimIO with the connection-name "check_ExportMesh"
    2. The solver calls "ExportMesh" with identifier "mesh_to_exchange" and exports the mesh that is defined below
    3. Run this script in a separate terminal. If it runs through then everything works as expected

For a python-solver this could be the code to check the ExportMesh function:

import CoSimIO

connection_name = "check_ExportMesh"
identifier = "mesh_to_exchange"

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

settings = {} # using default settings
CoSimIO.Connect(connection_name, settings)

# checking if the ExportMesh function works as expected
CoSimIO.ExportMesh(connection_name, identifier, nodal_coords, element_connectivities, element_types)

CoSimIO.Disconnect(connection_name)
'''

import CoSimIO

connection_name = "check_ExportMesh"
identifier = "mesh_to_exchange"

settings = {"is_connection_master" : "1"} # "is_connection_master" should only be used by Kratos!
CoSimIO.Connect(connection_name, settings)

# the solver to be checked calls "ExportMesh", hence here we call the counterpart "ImportMesh"
nodal_coords, element_connectivities, element_types = CoSimIO.ImportMesh(connection_name, identifier)
print("Mesh was imported")

'''
  -- Expected Mesh --
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

expected_nodal_coords = [
    0.0, 2.5, 1.0, # 0
    2.0, 0.0, 1.5, # 1
    2.0, 2.5, 1.5, # 2
    4.0, 2.5, 1.7, # 3
    4.0, 0.0, 1.7, # 4
    6.0, 0.0, 1.8  # 5
]

expected_element_connectivities = [
    0, 1, 2, # 1
    1, 3, 2, # 2
    1, 4, 3, # 3
    3, 4, 5, # 4
];

expected_element_types = [
    5,5,5,5 # VTK_TRIANGLE
]

def Compare(values_list, expected_values_list):
    size_values_list = len(values_list)
    size_expected_values_list = len(expected_values_list)

    if size_values_list != size_expected_values_list:
        raise Exception('Expected to import {} values but got {} instead!'.format(size_expected_values_list, size_values_list))

    for i, (v_imported, v_expected) in enumerate(zip(values_list, expected_values_list)):
        if abs(v_imported-v_expected) > 1e-12:
            raise Exception('Expected a value of {} in position {} but got {} instead!'.format(v_expected, i, v_imported))

Compare(nodal_coords, expected_nodal_coords)
Compare(element_connectivities, expected_element_connectivities)
Compare(element_types, expected_element_types)

CoSimIO.Disconnect(connection_name)

print("Importing Mesh Successful!")
