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

// Project includes
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
    settings.Set("my_name", "cpp_mesh_import_solver");
    settings.Set("connect_to", "cpp_mesh_export_solver");
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    (void)(argc);
    (void)(argv);

    auto info = CoSimIO::Connect(settings);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    const std::vector<std::array<double,3>> expected_nodal_coords {
        {0.0, 2.5, 1.0},
        {2.0, 0.0, 1.5},
        {2.0, 2.5, 1.5},
        {4.0, 2.5, 1.7},
        {4.0, 0.0, 1.7},
        {6.0, 0.0, 1.8}
    };

    const std::vector<CoSimIO::ConnectivitiesType> expected_element_connectivities {
        {1, 2, 3},
        {2, 4, 3},
        {2, 5, 4},
        {4, 5, 6},
    };

    info.Clear();
    info.Set("identifier", "fluid_mesh");
    info.Set("connection_name", connection_name);

    CoSimIO::ModelPart model_part("mp_exchange");

    info = CoSimIO::ImportMesh(info, model_part);

    COSIMIO_CHECK_EQUAL(model_part.NumberOfNodes(), expected_nodal_coords.size());
    COSIMIO_CHECK_EQUAL(model_part.NumberOfElements(), expected_element_connectivities.size());

    int counter=0;
    for (auto node_it=model_part.NodesBegin(); node_it!=model_part.NodesEnd(); ++node_it) {
        COSIMIO_CHECK_EQUAL((*node_it)->Id(), counter+1);
        for (std::size_t i=0; i<3; ++i) {
            COSIMIO_CHECK_EQUAL((*node_it)->Coordinates()[i], expected_nodal_coords[counter][i]);
        }
        counter++;
    }

    counter=0;
    for (auto elem_it=model_part.ElementsBegin(); elem_it!=model_part.ElementsEnd(); ++elem_it) {
        COSIMIO_CHECK_EQUAL((*elem_it)->Id(), counter+1);
        COSIMIO_CHECK_EQUAL(static_cast<int>((*elem_it)->Type()), static_cast<int>(CoSimIO::ElementType::Triangle3D3));
        COSIMIO_CHECK_EQUAL((*elem_it)->NumberOfNodes(), 3);
        int node_counter=0;
        for (auto node_it=(*elem_it)->NodesBegin(); node_it!=(*elem_it)->NodesEnd(); ++node_it) {
            COSIMIO_CHECK_EQUAL((*node_it)->Id(), expected_element_connectivities[counter][node_counter++]);
        }
        counter++;
    }

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    return 0;
}
