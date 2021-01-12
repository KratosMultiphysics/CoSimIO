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
#include "impl/communication/communication.hpp"

namespace {

template<class TCommType>
void ConnectDisconnect()
{
    CoSimIO::Info settings;

    settings.Set<std::string>("my_name", "thread");
    settings.Set<std::string>("connect_to", "main");
    settings.Set<bool>("is_primary_connection", false);
    settings.Set<int>("echo_level", 0);

    using Communication = CoSimIO::Internals::Communication;
    std::unique_ptr<Communication> p_comm(CoSimIO::make_unique<TCommType>(settings));

    // the secondary thread should wait a bit until the primary has created the folder!
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    CoSimIO::Info connect_info;
    CoSimIO::Info ret_info_connect = p_comm->Connect(connect_info);

    CHECK_UNARY(ret_info_connect.Get<bool>("is_connected"));

    CoSimIO::Info disconnect_info;
    CoSimIO::Info ret_info_disconnect = p_comm->Disconnect(disconnect_info);

    CHECK_UNARY_FALSE(ret_info_disconnect.Get<bool>("is_connected"));
}

template<class TCommType>
void ExportInfoHelper(const std::size_t NumExports)
{
    CoSimIO::Info settings;

    settings.Set<std::string>("my_name", "thread");
    settings.Set<std::string>("connect_to", "main");
    settings.Set<bool>("is_primary_connection", false);

    using Communication = CoSimIO::Internals::Communication;
    std::unique_ptr<Communication> p_comm(CoSimIO::make_unique<TCommType>(settings));

    // the secondary thread should wait a bit until the primary has created the folder!
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    CoSimIO::Info connect_info;
    CoSimIO::Info ret_info_connect = p_comm->Connect(connect_info);

    CHECK_UNARY(ret_info_connect.Get<bool>("is_connected"));

    CoSimIO::Info info_for_export;
    info_for_export.Set<std::string>("some_random_name", "the_CoSimIO");
    info_for_export.Set<bool>("is_converged", true);
    info_for_export.Set<int>("echo_level", 2);

    for (std::size_t i=0; i<NumExports; ++i) {
        info_for_export.Set<double>("tol", 0.008*(i+1));
        info_for_export.Set<int>("counter", i+3);
        p_comm->ExportInfo(info_for_export);
    }

    CoSimIO::Info disconnect_info;
    CoSimIO::Info ret_info_disconnect = p_comm->Disconnect(disconnect_info);

    CHECK_UNARY_FALSE(ret_info_disconnect.Get<bool>("is_connected"));
}

}

// neither of the tests should take more than 1.0 seconds. If it does it means that it hangs!
TEST_CASE_TEMPLATE_DEFINE("Communication"* doctest::timeout(1.0), TCommType, COMM_TESTS)
{
    CoSimIO::Info settings;

    settings.Set<std::string>("my_name", "main");
    settings.Set<std::string>("connect_to", "thread");
    settings.Set<bool>("is_primary_connection", true);
    settings.Set<int>("echo_level", 0);

    using Communication = CoSimIO::Internals::Communication;
    std::unique_ptr<Communication> p_comm(CoSimIO::make_unique<TCommType>(settings));

    SUBCASE("connect_disconnect_once")
    {
        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        std::thread ext_thread(ConnectDisconnect<TCommType>);
        ext_thread.join();

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);
    }

    SUBCASE("connect_disconnect_multiple")
    {
        // connecting and disconnecting three times
        for (std::size_t i=0; i<3; ++i) {
            CoSimIO::Info connect_info;
            p_comm->Connect(connect_info);

            std::thread ext_thread(ConnectDisconnect<TCommType>);
            ext_thread.join();

            CoSimIO::Info disconnect_info;
            p_comm->Disconnect(disconnect_info);
        }
    }

    SUBCASE("import_export_info_once")
    {
        std::thread ext_thread(ExportInfoHelper<TCommType>, 1);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        CoSimIO::Info import_info;
        auto imported_info = p_comm->ImportInfo(import_info);

        CHECK_UNARY(imported_info.Has("some_random_name"));
        CHECK_UNARY(imported_info.Has("is_converged"));
        CHECK_UNARY(imported_info.Has("tol"));
        CHECK_UNARY(imported_info.Has("counter"));

        CHECK_EQ(imported_info.Get<std::string>("some_random_name"), "the_CoSimIO");
        CHECK_UNARY(imported_info.Get<bool>("is_converged"));
        CHECK_EQ(imported_info.Get<double>("tol"), doctest::Approx(0.008));
        CHECK_EQ(imported_info.Get<int>("counter"), 3);

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
    }

    SUBCASE("import_export_info_multiple")
    {
        const std::size_t num_exports = 4;
        std::thread ext_thread(ExportInfoHelper<TCommType>, num_exports);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        for (std::size_t i=0; i<num_exports; ++i) {
            CoSimIO::Info import_info;
            auto imported_info = p_comm->ImportInfo(import_info);

            CHECK_UNARY(imported_info.Has("some_random_name"));
            CHECK_UNARY(imported_info.Has("is_converged"));
            CHECK_UNARY(imported_info.Has("tol"));
            CHECK_UNARY(imported_info.Has("counter"));

            CHECK_EQ(imported_info.Get<std::string>("some_random_name"), "the_CoSimIO");
            CHECK_UNARY(imported_info.Get<bool>("is_converged"));
            CHECK_EQ(imported_info.Get<double>("tol"), doctest::Approx(0.008*(i+1)));
            CHECK_EQ(imported_info.Get<int>("counter"), i+3);
        }

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
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
}

// Registering tests for different types of Communication
#include "impl/communication/file_communication.hpp"
using FileCommunication = CoSimIO::Internals::FileCommunication;
TYPE_TO_STRING(FileCommunication);

TEST_CASE_TEMPLATE_INVOKE(COMM_TESTS, FileCommunication);

#ifdef CO_SIM_IO_USING_SOCKETS
#include "impl/communication/sockets_communication.hpp"
using SocketsCommunication = CoSimIO::Internals::SocketsCommunication;
TYPE_TO_STRING(SocketsCommunication);
TEST_CASE_TEMPLATE_INVOKE(COMM_TESTS, SocketsCommunication);
#endif

#ifdef CO_SIM_IO_USING_MPI
#include "impl/communication/mpi_communication.hpp"
using MPICommunication = CoSimIO::Internals::MPICommunication;
TYPE_TO_STRING(MPICommunication);
TEST_CASE_TEMPLATE_INVOKE(COMM_TESTS, MPICommunication);
#endif
