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

// External includes
#include "mpi.h"

// CoSimulation includes
#include "co_sim_io_mpi.hpp"
DEFINE_SINGLETON_MAIN( );

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    CoSimIO::Info settings;
    settings.Set("my_name", "cpp_connect_disconnect_a");
    settings.Set("connect_to", "cpp_connect_disconnect_b");
    settings.Set("is_distributed", true);
    settings.Set("echo_level", 1);
    settings.Set("version", "1.25");

    auto info = CoSimIO::ConnectMPI(settings, MPI_COMM_WORLD);
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
    const std::string connection_name = info.Get<std::string>("connection_name");

    CoSimIO::Info disconnect_settings;
    disconnect_settings.Set("connection_name", connection_name);
    info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
    COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);

    MPI_Finalize();

    return 0;
}
