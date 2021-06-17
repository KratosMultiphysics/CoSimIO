# Tutorial for exchanging meshes with Kratos

This tutorial shows how to exchange meshes with Kratos. It is required to do [this tutorial](basic_data_exchange_with_kratos.md) first.

## Mesh Exchange with Kratos
Here we try to send some data to Kratos and get it back from it. Then we can check if both data are the same. Again the python file for Kratos side is very similar to the one described in the [python tutorial](../python/integration_co_sim_io.md):

~~~py
import KratosMultiphysics as KM
from KratosMultiphysics.CoSimulationApplication import CoSimIO

model = KM.Model()
model_part = model.CreateModelPart("mp_test")

connection_settings = CoSimIO.Info()
connection_settings.SetString("my_name", "Kratos")
connection_settings.SetString("connect_to", "my_code")
connection_settings.SetInt("echo_level", 0)
info = CoSimIO.Connect(connection_settings)
connection_name = info.GetString("connection_name")
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Connected:
    raise Exception("Connecting failed")

import_info = CoSimIO.Info()
import_info.SetString("connection_name", connection_name)
import_info.SetString("identifier", "mesh_exchange_1")
CoSimIO.ImportMesh(import_info, model_part)

# print(model_part)

export_info = CoSimIO.Info()
export_info.SetString("connection_name", connection_name)
export_info.SetString("identifier", "mesh_exchange_2")
CoSimIO.ExportMesh(export_info, model_part)

disconnect_settings = CoSimIO.Info()
disconnect_settings.SetString("connection_name", connection_name)

info = CoSimIO.Disconnect(disconnect_settings)
if info.GetInt("connection_status") != CoSimIO.ConnectionStatus.Disconnected:
    raise Exception("Disconnecting failed")

~~~

You may find this python file in [here](https://github.com/KratosMultiphysics/Kratos/blob/master/applications/CoSimulationApplication/tests/co_sim_io_py_exposure_aux_files/import_export_mesh.py)

On the other side we use first export mesh and then import it back, following what was done in [this tutorial](integration_co_sim_io.md#mesh-exchange):

~~~c++
// System includes
#include <cmath> // std::abs

// CoSimulation includes
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "my_code");
    settings.Set("connect_to", "Kratos");
    settings.Set("echo_level", 1);

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    // send mesh to Kratos
    CoSimIO::ModelPart model_part("my_mesh");

    // fill the ModelPart with nodes & elements ...
    for (int i=0; i<10; ++i) {
        model_part.CreateNewNode(i+1, 1.1*i,0,0);
    }

    for (int i=0; i<5; ++i) {
        model_part.CreateNewElement(i+1, CoSimIO::ElementType::Line2D2, {i+1, i+2});
    }

    info.Clear();
    info.Set("identifier", "mesh_exchange_1");
    info.Set("connection_name", connection_name);
    info = CoSimIO::ExportMesh(info, model_part);

    // receive the same data from Kratos
    CoSimIO::ModelPart model_part_received("my_mesh_2");
    info.Clear();
    info.Set("identifier", "mesh_exchange_1");
    info.Set("connection_name", connection_name);
    info = CoSimIO::ImportMesh(info, model_part_received);

    // check that the meshes are the same
    // e.g. similar to "CoSimIO/tests/co_sim_io/impl/co_sim_io_testing.hpp"

    if (model_part.NumberOfNodes() != model_part_received.NumberOfNodes()) {
        return 1;
    }

    if (model_part.NumberOfElements() != model_part_received.NumberOfElements()) {
        return 1;
    }

    // do more detailed checks if necessary ...

    // disconnecting afterwards
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
~~~

This example can be found in [integration_tutorials/cpp/mesh_exchange.cpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests/integration_tutorials/cpp/mesh_exchange.cpp)

Then you may run your executable with python script of Kratos from your working directory:

~~~shell
path/to/bin/mesh_exchange_cpp_test & python3 path/to/connect_disconnect.py
~~~

## Next steps
In the [next tutorial](data_and_mesh_exchange_and_mapping.md), meshes are exchanged with Kratos and mapping is used.
