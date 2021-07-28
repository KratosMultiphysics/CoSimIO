# importing the Kratos library
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO
import KratosMultiphysics.MappingApplication as KratosMapping

# create the Kratos ModelParts that contain the mesh
model = KM.Model()
model_part_origin = model.CreateModelPart("mp_origin")
model_part_destination = model.CreateModelPart("mp_destination")

# allocate memory
model_part_origin.AddNodalSolutionStepVariable(KM.TEMPERATURE)
model_part_destination.AddNodalSolutionStepVariable(KM.AMBIENT_TEMPERATURE)

model_part_origin.AddNodalSolutionStepVariable(KM.VELOCITY)
model_part_destination.AddNodalSolutionStepVariable(KM.MESH_VELOCITY)

# connect to CoSimIO
connection_settings = CoSimIO.Info()
connection_settings.SetString("connection_name", "mesh_mapping")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

# import meshes
import_mesh_info_o = CoSimIO.Info()
import_mesh_info_o.SetString("connection_name", "mesh_mapping")
import_mesh_info_o.SetString("identifier", "mesh_origin")
CoSimIO.ImportMesh(import_mesh_info_o, model_part_origin)

import_mesh_info_d = CoSimIO.Info()
import_mesh_info_d.SetString("connection_name", "mesh_mapping")
import_mesh_info_d.SetString("identifier", "mesh_destination")
CoSimIO.ImportMesh(import_mesh_info_d, model_part_destination)

# printing the ModelParts for debugging:
# print(model_part_origin)
# print(model_part_destination)

# input for the mapper is a Kratos::Parameters object
mapper_settings = KM.Parameters("""{
    "mapper_type": "nearest_neighbor",
    "echo_level" : 0
}""")

# creating the mapper using the mapper factory
mapper = KratosMapping.MapperFactory.CreateMapper(
    model_part_origin,
    model_part_destination,
    mapper_settings)

# import data to be mapped
import_data_info = CoSimIO.Info()
import_data_info.SetString("connection_name", "mesh_mapping")
import_data_info.SetString("identifier", "data_to_map")
CoSimIO.ImportData(import_data_info, model_part_origin, KM.TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)

# map scalar quantities
mapper.Map(KM.TEMPERATURE, KM.AMBIENT_TEMPERATURE)

# map vector quantities
mapper.Map(KM.VELOCITY, KM.MESH_VELOCITY)

# export mapped data
export_data_info = CoSimIO.Info()
export_data_info.SetString("connection_name", "mesh_mapping")
export_data_info.SetString("identifier", "mapped_data")
CoSimIO.ExportData(export_data_info, model_part_destination, KM.AMBIENT_TEMPERATURE, CoSimIO.DataLocation.NodeHistorical)

# disconnect from CoSimIO
disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", "mesh_mapping")

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")