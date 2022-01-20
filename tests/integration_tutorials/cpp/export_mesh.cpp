//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Pooyan Dadvand
//

// System includes
#include <vector>
#include <array>
#include <cmath>

// Project includes
#include "co_sim_io.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }


std::string HumanReadableSize(std::size_t InBytes) {
    constexpr char extension[] = {'\0', 'K', 'M', 'G', 'T', 'P', 'E', 'E'};

    std::stringstream output;
    output.precision(4);

    double output_size = static_cast<double>(InBytes);
    int i = 0;
    for (; i < 7; i++) {
      if (output_size < 1024) {
          break;
      }
      output_size /= 1024;
    }
    output << output_size << " " << extension[i] << 'B';
    return output.str();
}

int main()
{
    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_mesh_export_solver");
    settings.Set("connect_to", "cpp_mesh_import_solver");
    settings.Set("communication_format", "pipe");
    settings.Set("use_file_serializer", false);
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    // Creating the mesh
    const std::vector<std::array<double,3>> nodal_coords {
        {0.0, 2.5, 1.0},
        {2.0, 0.0, 1.5},
        {2.0, 2.5, 1.5},
        {4.0, 2.5, 1.7},
        {4.0, 0.0, 1.7},
        {6.0, 0.0, 1.8}
    };

    const std::vector<CoSimIO::ConnectivitiesType> element_connectivities {
        {1, 2, 3},
        {2, 4, 3},
        {2, 5, 4},
        {4, 5, 6},
    };

    CoSimIO::ModelPart model_part("mp_exchange");

    int id_counter=0;
    for (const auto& coords : nodal_coords) {
        model_part.CreateNewNode(++id_counter, coords[0], coords[1], coords[2]);
    }

    id_counter=0;
    for (const auto& conn : element_connectivities) {
        model_part.CreateNewElement(++id_counter, CoSimIO::ElementType::Triangle3D3, conn);
    }

    id_counter = model_part.NumberOfNodes()+5;
    for (std::size_t i=0;i<1e8;++i) {
        id_counter++;
        model_part.CreateNewNode(i+id_counter, 0,0,0);
    }

    info.Clear();
    info.Set("identifier", "fluid_mesh");
    info.Set("connection_name", connection_name);

    info = CoSimIO::ExportMesh(info, model_part);

    std::cout << "    Elapsed time: " << info.Get<double>("elapsed_time") << std::endl;
    std::cout << "    Memory usage: " << HumanReadableSize(info.Get<std::size_t>("memory_usage_ipc")) << std::endl;
    if (info.Has("elapsed_time_ipc")) {
        std::cout << "    IPC time: " << info.Get<double>("elapsed_time_ipc") << std::endl;
        std::cout << "    Serializer time: " << info.Get<double>("elapsed_time_serializer") << std::endl;
    }

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
