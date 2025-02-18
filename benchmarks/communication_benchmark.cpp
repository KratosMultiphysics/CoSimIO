//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz

// System includes
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <cassert>

// External includes
#include <benchmark/benchmark.h>

// Project includes
#include "includes/communication/communication.hpp"
#include "includes/communication/factory.hpp"

namespace CoSimIO
{

//--------------------------------------------------------------------------
// Helper: ExportDataHelper from tests/co_sim_io/cpp/test_communication.cpp
//--------------------------------------------------------------------------
void ExportDataHelper(
    Info settings,
    const std::vector<std::vector<double>>& DataToExport)
{
    settings.Set<std::string>("my_name", "thread");
    settings.Set<std::string>("connect_to", "main");
    settings.Set<bool>("is_primary_connection", false);
    settings.Set<int>("echo_level", 0);

    using Communication = Internals::Communication;
    std::unique_ptr<Communication> p_comm = 
        Internals::CommunicationFactory().Create(
            settings, std::make_shared<Internals::DataCommunicator>());

    // Give the primary thread a chance to prepare (as in your test)
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    Info connect_info;
    p_comm->Connect(connect_info);

    Info export_info;
    export_info.Set<std::string>("identifier", "data_exchange");

    for (const auto& data : DataToExport) {
        Internals::DataContainerStdVectorReadOnly<double> data_container(data);
        p_comm->ExportData(export_info, data_container);
    }

    Info disconnect_info;
    p_comm->Disconnect(disconnect_info);
}

//--------------------------------------------------------------------------
// Google Benchmark for "import_export_large_data" from tests/co_sim_io/cpp/test_communication.cpp
//--------------------------------------------------------------------------
static void BM_ImportExportLargeData(benchmark::State& state) {
    // Get size in MB from benchmark parameter.
    const std::size_t size_MB = static_cast<std::size_t>(state.range(0));
    const std::size_t size_B = size_MB * 1024 * 1024;
    const std::size_t size_vec = size_B / sizeof(double);

    // Prepare the large data vector (size_MB MB of doubles)
    std::vector<std::vector<double>> exp_data(1);
    exp_data[0].resize(size_vec);
    std::iota(exp_data[0].begin(), exp_data[0].end(), 0);

    // Set up common communication settings
    Info settings;
    settings.Set<std::string>("communication_format", "pipe");
    // You can add more settings if needed

    // Benchmark loop: each iteration performs a full connect-import-disconnect cycle
    for (auto _ : state) {
        using Communication = Internals::Communication;
        std::unique_ptr<Communication> main_comm = 
            Internals::CommunicationFactory().Create(
                settings, std::make_shared<Internals::DataCommunicator>());

        // Launch the exporter in a separate thread (it uses ExportDataHelper)
        std::thread export_thread(ExportDataHelper, settings, exp_data);

        // Connect the main (import) side
        Info connect_info;
        main_comm->Connect(connect_info);

        // Prepare a container for the imported data
        std::vector<double> imported_data;
        Internals::DataContainerStdVector<double> data_container(imported_data);

        Info import_info;
        import_info.Set<std::string>("identifier", "data_exchange");

        // Perform the import operation (this is what we want to measure)
        main_comm->ImportData(import_info, data_container);

        // Validate that the imported data has the expected size.
        assert(data_container.GetData().size() == exp_data[0].size());

        // Disconnect the main communication
        Info disconnect_info;
        main_comm->Disconnect(disconnect_info);

        // Ensure the exporter thread completes before starting the next iteration
        export_thread.join();
    }
}

// Register the benchmark with different sizes in MB (e.g., 5 MB, 10 MB, 20 MB)
BENCHMARK(BM_ImportExportLargeData)->Arg(5)->Arg(10)->Arg(20);

}  // namespace CoSimIO

BENCHMARK_MAIN();