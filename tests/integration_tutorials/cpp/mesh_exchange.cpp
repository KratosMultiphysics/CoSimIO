//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

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

int main(int argc, const char** argv)
{
    CoSimIO::Info settings;
    settings.Set("my_name", "my_code");
    settings.Set("connect_to", "Kratos");
    settings.Set("echo_level", 1);

    (void)(argc);
    (void)(argv);

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

    // receive the same mesh from Kratos
    CoSimIO::ModelPart model_part_received("my_mesh_2");
    info.Clear();
    info.Set("identifier", "mesh_exchange_1");
    info.Set("connection_name", connection_name);
    info = CoSimIO::ImportMesh(info, model_part_received);

    // check that the meshes are the same
    // e.g. similar to "CoSimIO/tests/co_sim_io/cpp/co_sim_io_testing.hpp"

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
