//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher
//

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
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_import_info_solver");
    settings.Set("connect_to", "cpp_export_info_solver");
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::ConnectMPI(settings, MPI_COMM_WORLD);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    CoSimIO::Info exp_info;
    exp_info.Set<std::string>("connection_name", connection_name);
    exp_info.Set<std::string>("identifier", "cpp_mpi_info_exchange");
    auto imported_info = CoSimIO::ImportInfo(exp_info);

    COSIMIO_CHECK_EQUAL(imported_info.Get<std::string>("id"), "convergence_information");
    COSIMIO_CHECK_EQUAL(imported_info.Get<bool>("is_converged"), true);
    COSIMIO_CHECK_EQUAL(imported_info.Get<double>("tol"), 0.008);
    COSIMIO_CHECK_EQUAL(imported_info.Get<int>("echo_level"), 2);

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    MPI_Finalize();

    return 0;
}
