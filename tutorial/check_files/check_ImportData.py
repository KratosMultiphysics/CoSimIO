'''
This file is intended to check the "ImportData" function of a Solver using CoSimIO
Requirements: compiling the Python-interface of CoSimIO (see the docs)

Instructions:
    1. The solver that is to be checked connects to CoSimIO with the connection-name "check_ImportData"
    2. The solver calls "ImportData" with identifier "data_to_exchange"
    3. Run this script in a separate terminal. If it runs through then everything works as expected
    4. Check the imported values. They should be the same as specified below as "values_to_exchange"

For a python-solver this could be the code to check the ImportData function:

import CoSimIO

connection_name = "check_ImportData"
identifier = "data_to_exchange"

settings = {} # using default settings
CoSimIO.Connect(connection_name, settings)

# checking if the ExportData function works as expected
imported_data = CoSimIO.ImportData(connection_name, identifier)

# now checking if the expected values match the imported ones
expected_values = [741.65, 88.3, 1.0, 2.5, -45.789, 10.3, 66.7]

CoSimIO.Disconnect(connection_name)
'''

import CoSimIO

connection_name = "check_ImportData"
identifier = "data_to_exchange"
values_to_exchange = [741.65, 88.3, 1.0, 2.5, -45.789, 10.3, 66.7]

settings = {"is_connection_master" : "1"} # "is_connection_master" should only be used by Kratos!
CoSimIO.Connect(connection_name, settings)

# the solver to be checked calls "ImportData", hence here we call the counterpart "ExportData"
CoSimIO.ExportData(connection_name, "data_to_exchange", values_to_exchange)
print("Data was exported:\n{}".format(values_to_exchange))

CoSimIO.Disconnect(connection_name)

print("Exporting Data Successful!")
