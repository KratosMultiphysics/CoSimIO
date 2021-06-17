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

    // // check that the data is the same:
    // if (data_to_send.size() != data_received.size()) {
    //     return 1;
    // }

    // for (std::size_t i=0; i<data_to_send.size(); ++i) {
    //     // compare if values are the same, with a small tolerance
    //     if (std::abs(data_to_send[i] - data_received[i]) > 1e-12) {
    //         return 1;
    //     }
    // }

    // disconnecting afterwards
    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
