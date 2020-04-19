'''
This file is intended to check the "ExportData" function of a Solver using CoSimIO
Requirements: compiling the Python-interface of CoSimIO (see the docs)

Instructions:
    1. The solver that is to be checked connects to CoSimIO with the connection-name "check_ExportData"
    2. The solver calls "ExportData" with identifier "data_to_exchange" and exports the
       values [1.0, 2.5, -45.789, 10.3, 55.7]
    3. Run this script in a separate terminal. If it runs through then everything works as expected

For a python-solver this could be the code to check the ExportData function:

import CoSimIO

connection_name = "check_ExportData"
identifier = "data_to_exchange"
values_to_exchange = [1.0, 2.5, -45.789, 10.3, 55.7]

settings = {} # using default settings
CoSimIO.Connect(connection_name, settings)

# checking if the ExportData function works as expected
CoSimIO.ExportData(connection_name, identifier, values_to_exchange)

CoSimIO.Disconnect(connection_name)
'''

import CoSimIO

connection_name = "check_ExportData"
identifier = "data_to_exchange"
values_to_exchange = [1.0, 2.5, -45.789, 10.3, 55.7]

settings = {"is_connection_master" : "1"} # "is_connection_master" should only be used by Kratos!
CoSimIO.Connect(connection_name, settings)

# the solver to be checked calls "ExportData", hence here we call the counterpart "ImportData"
imported_data = CoSimIO.ImportData(connection_name, "data_to_exchange")
print("Data was imported:\n{}".format(imported_data))

size_imported_data = len(imported_data)
size_values_to_exchange = len(values_to_exchange)

if size_imported_data != size_values_to_exchange:
    raise Exception('Expected to import {} values but got {} instead!'.format(size_values_to_exchange, size_imported_data))

for i, (v_imported, v_expected) in enumerate(zip(imported_data, values_to_exchange)):
    if abs(v_imported-v_expected) > 1e-12:
        raise Exception('Expected a value of {} in position {} but got {} instead!'.format(v_expected, i, v_imported))

CoSimIO.Disconnect(connection_name)

print("Importing Data Successful!")
