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
#include <vector>
#include <array>

// External includes
#include "mpi.h"

// CoSimulation includes
#include "co_sim_io_mpi.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv); // needs to be done before calling CoSimIO::ConnectMPI

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_mesh_export_solver");
    settings.Set("connect_to", "cpp_mesh_import_solver");
#ifdef CO_SIM_IO_BUILD_MPI_COMMUNICATION
    settings.Set("communication_format", "mpi_inter");
#endif
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::ConnectMPI(settings, MPI_COMM_WORLD);
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

    info.Clear();
    info.Set("identifier", "fluid_mesh");
    info.Set("connection_name", connection_name);

    info = CoSimIO::ExportMesh(info, model_part);

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    MPI_Finalize();

    return 0;
}
