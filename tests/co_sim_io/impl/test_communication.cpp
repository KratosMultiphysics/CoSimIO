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

void CheckNodesAreEqual(
    const CoSimIO::Node& Node1,
    const CoSimIO::Node& Node2)
{
    CHECK_EQ(Node1.Id(), Node2.Id());
    CHECK_EQ(Node1.X(), doctest::Approx(Node2.X()));
    CHECK_EQ(Node1.Y(), doctest::Approx(Node2.Y()));
    CHECK_EQ(Node1.Z(), doctest::Approx(Node2.Z()));
}

void CheckElementsAreEqual(
    const CoSimIO::Element& Element1,
    const CoSimIO::Element& Element2)
{
    // basic checks
    CHECK_EQ(Element1.Id(), Element2.Id());
    CHECK_EQ(Element1.Type(), Element2.Type());
    REQUIRE_EQ(Element1.NumberOfNodes(), Element2.NumberOfNodes());

    // check nodes
    for (std::size_t i=0; i< Element1.NumberOfNodes(); ++i) {
        CheckNodesAreEqual(**(Element1.NodesBegin()+i), **(Element2.NodesBegin()+i));
    }
}

void CheckModelPartsAreEqual(
    const CoSimIO::ModelPart& ModelPart1,
    const CoSimIO::ModelPart& ModelPart2)
{
    // basic checks
    CHECK_EQ(ModelPart1.Name(), ModelPart2.Name());
    REQUIRE_EQ(ModelPart1.NumberOfNodes(), ModelPart2.NumberOfNodes());
    REQUIRE_EQ(ModelPart1.NumberOfElements(), ModelPart2.NumberOfElements());

    // check nodes
    for (std::size_t i=0; i< ModelPart1.NumberOfNodes(); ++i) {
        CheckNodesAreEqual(**(ModelPart1.NodesBegin()+i), **(ModelPart2.NodesBegin()+i));
    }

    // check elements
    for (std::size_t i=0; i< ModelPart1.NumberOfElements(); ++i) {
        CheckElementsAreEqual(**(ModelPart1.ElementsBegin()+i), **(ModelPart2.ElementsBegin()+i));
    }
}

std::shared_ptr<CoSimIO::ModelPart> CreateNodesOnlyModelPart()
{
    std::shared_ptr<CoSimIO::ModelPart> p_model_part(std::make_shared<CoSimIO::ModelPart>("nodes_model_part"));

    const int num_nodes = 11;

    for (int i=0; i<num_nodes; ++i) {
        p_model_part->CreateNewNode(i+i+1, 0.1*i, 0, 0);
    }

    return p_model_part;
}

std::shared_ptr<CoSimIO::ModelPart> CreateLinesModelPart()
{
    std::shared_ptr<CoSimIO::ModelPart> p_model_part(std::make_shared<CoSimIO::ModelPart>("line_model_part"));

    const int num_nodes = 13;

    for (int i=0; i<num_nodes; ++i) {
        p_model_part->CreateNewNode(i+i+1, 0.1*i, 0, 0);
    }

    for (int i=0; i<num_nodes-1; ++i) {
        p_model_part->CreateNewElement(i+1, 3, {i+i+1, i+i+3});
    }

    return p_model_part;
}

std::shared_ptr<CoSimIO::ModelPart> CreateLinesAndPointElementsModelPart()
{
    std::shared_ptr<CoSimIO::ModelPart> p_model_part(std::make_shared<CoSimIO::ModelPart>("line_points_model_part"));

    CO_SIM_IO_ERROR << "not implemented!" << std::endl;

    return p_model_part;
}

std::shared_ptr<CoSimIO::ModelPart> CreateSurfaceModelPart()
{
    std::shared_ptr<CoSimIO::ModelPart> p_model_part(std::make_shared<CoSimIO::ModelPart>("surface_model_part"));

    CO_SIM_IO_ERROR << "not implemented!" << std::endl;

    return p_model_part;
}

std::shared_ptr<CoSimIO::ModelPart> CreateVolumeModelPart()
{
    std::shared_ptr<CoSimIO::ModelPart> p_model_part(std::make_shared<CoSimIO::ModelPart>("volume_model_part"));

    CO_SIM_IO_ERROR << "not implemented!" << std::endl;

    return p_model_part;
}

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
    settings.Set<int>("echo_level", 0);

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

    for (std::size_t i=0; i<NumExports; ++i) {
        info_for_export.Set<double>("tol", 0.008*(i+1));
        info_for_export.Set<int>("counter", static_cast<int>(i)+3);
        p_comm->ExportInfo(info_for_export);
    }

    CoSimIO::Info disconnect_info;
    CoSimIO::Info ret_info_disconnect = p_comm->Disconnect(disconnect_info);

    CHECK_UNARY_FALSE(ret_info_disconnect.Get<bool>("is_connected"));
}

template<class TCommType>
void ExportDataHelper(const std::vector<std::vector<double>>& DataToExport)
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

    CoSimIO::Info export_info;
    export_info.Set<std::string>("identifier", "data_exchange");

    for (const auto& data : DataToExport) {
        const CoSimIO::Internals::DataContainerStdVectorReadOnly<double> data_container(data);
        p_comm->ExportData(export_info, data_container);
    }

    CoSimIO::Info disconnect_info;
    CoSimIO::Info ret_info_disconnect = p_comm->Disconnect(disconnect_info);

    CHECK_UNARY_FALSE(ret_info_disconnect.Get<bool>("is_connected"));
}

template<class TCommType>
void ExportMeshHelper(const std::vector<std::shared_ptr<CoSimIO::ModelPart>>& ModelPartsToExport)
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

    CoSimIO::Info export_info;

    for (const auto& model_part : ModelPartsToExport) {
        p_comm->ExportMesh(export_info, *model_part);
    }

    CoSimIO::Info disconnect_info;
    CoSimIO::Info ret_info_disconnect = p_comm->Disconnect(disconnect_info);

    CHECK_UNARY_FALSE(ret_info_disconnect.Get<bool>("is_connected"));
}

}

// neither of the tests should take more than 5.0 seconds. If it does it means that it hangs!
TEST_CASE_TEMPLATE_DEFINE("Communication"* doctest::timeout(5.0), TCommType, COMM_TESTS)
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
        const std::vector<std::vector<double>> exp_data {
            {1.0, -6.1, 55.789, 547}
        };
        std::thread ext_thread(ExportDataHelper<TCommType>, exp_data);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        std::vector<double> data;
        CoSimIO::Internals::DataContainerStdVector<double> data_container(data);

        CoSimIO::Info import_info;
        import_info.Set<std::string>("identifier", "data_exchange");
        p_comm->ImportData(import_info, data_container);

        CO_SIM_IO_CHECK_VECTOR_NEAR(data_container, exp_data[0]);

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
    }

    SUBCASE("import_export_data_multiple")
    {
        const std::vector<std::vector<double>> exp_data {
            {1.1, -6.1, 535.789, 5487},
            {1.2, -6.01, 552.789, 5477, 1.0, -6.19, -655.789, 91.5888867},
            {1.3, -6.001, -655.789, 91.567},
            {1.4, -6.0001, 551.789, 5647, -1.0, 44.5, -876.123, -6.1, -63455.789, 91.567},
            {-11.56}
        };
        std::thread ext_thread(ExportDataHelper<TCommType>, exp_data);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        std::vector<double> data;
        CoSimIO::Internals::DataContainerStdVector<double> data_container(data);

        CoSimIO::Info import_info;
        import_info.Set<std::string>("identifier", "data_exchange");
        for (std::size_t i=0; i<exp_data.size(); ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            p_comm->ImportData(import_info, data_container);
            CO_SIM_IO_CHECK_VECTOR_NEAR(data_container, exp_data[i]);
        }

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
    }

    SUBCASE("import_export_mesh_once")
    {
        const std::vector<std::shared_ptr<CoSimIO::ModelPart>> model_parts {
            CreateLinesAndPointElementsModelPart()
        };
        std::thread ext_thread(ExportMeshHelper<TCommType>, model_parts);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        CoSimIO::Info import_info;
        CoSimIO::ModelPart imported_model_part(model_parts[0]->Name());
        p_comm->ImportMesh(import_info, imported_model_part);

        CheckModelPartsAreEqual(*model_parts[0], imported_model_part);

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
    }

    SUBCASE("import_export_mesh_multiple")
    {
        const std::vector<std::shared_ptr<CoSimIO::ModelPart>> model_parts {
            CreateNodesOnlyModelPart(),
            CreateLinesModelPart(),
            CreateLinesAndPointElementsModelPart(),
            CreateSurfaceModelPart(),
            CreateVolumeModelPart()
        };
        std::thread ext_thread(ExportMeshHelper<TCommType>, model_parts);

        CoSimIO::Info connect_info;
        p_comm->Connect(connect_info);

        CoSimIO::Info import_info;
        for (std::size_t i=0; i<model_parts.size(); ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CoSimIO::ModelPart imported_model_part(model_parts[i]->Name());
            p_comm->ImportMesh(import_info, imported_model_part);
            CheckModelPartsAreEqual(*model_parts[i], imported_model_part);
        }

        CoSimIO::Info disconnect_info;
        p_comm->Disconnect(disconnect_info);

        ext_thread.join();
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
