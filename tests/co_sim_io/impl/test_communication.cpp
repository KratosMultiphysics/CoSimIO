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
#include <thread>
#include <chrono>

// Project includes
#include "co_sim_io_testing.hpp"

// Communication includes
#include "impl/communication/file_communication.hpp"
using FileCommunication = CoSimIO::Internals::FileCommunication;
TYPE_TO_STRING(FileCommunication);

#ifdef CO_SIM_IO_USING_SOCKETS
#include "impl/communication/sockets_communication.hpp"
using SocketsCommunication = CoSimIO::Internals::SocketsCommunication;
TYPE_TO_STRING(SocketsCommunication);
#endif

#ifdef CO_SIM_IO_USING_MPI
#include "impl/communication/mpi_communication.hpp"
using MPICommunication = CoSimIO::Internals::MPICommunication;
TYPE_TO_STRING(MPICommunication);
#endif


TEST_CASE_TEMPLATE("Communication"* doctest::timeout(0.5), TComm, // neither of the tests should take more than 0.5 seconds. If it does it means that it hangs!
    FileCommunication
#ifdef CO_SIM_IO_USING_SOCKETS
    ,SocketsCommunication
#endif
#ifdef CO_SIM_IO_USING_MPI
    ,MPICommunication
#endif
    )
{
    SUBCASE("connect_disconnect_once")
    {
    }

    SUBCASE("connect_disconnect_multiple")
    {
    }

    SUBCASE("import_export_info_once")
    {
    }

    SUBCASE("import_export_info_multiple")
    {
    }

    SUBCASE("import_export_data_once")
    {
    }

    SUBCASE("import_export_data_multiple")
    {
    }

    SUBCASE("import_export_mesh_once")
    {
    }

    SUBCASE("import_export_mesh_multiple")
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait 0.5s before next check
    }

    // TComm("sss", CoSimIO::Info(), true);
}
