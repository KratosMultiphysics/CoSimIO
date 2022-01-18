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

// CoSimulation includes
#include "co_sim_io.hpp"

#include "data_exchange_testing_matrix.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }


int main()
{
    int counter = 0;
    for (CoSimIO::Info config : GetTestingMatrix()) {
        std::cout << "Current configuration:\n" << config << std::endl;
        const std::string my_name = "cpp_export_solver_" + std::to_string(counter);
        const std::string connect_to = "cpp_import_solver_" + std::to_string(counter);
        counter++;

        config.Set("my_name", my_name);
        config.Set("connect_to", connect_to);

        auto info = CoSimIO::Connect(config);
        COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
        const std::string connection_name = info.Get<std::string>("connection_name");

        std::vector<double> data_to_send(1e8, 3.14);

        double accum_time = 0.0;
        std::size_t accum_mem = 0;

        for (int i=0; i<NUM_EVALUATIONS+1; ++i) {
            info.Clear();
            info.Set("identifier", "vector_of_pi");
            info.Set("connection_name", connection_name);
            info = CoSimIO::ExportData(info, data_to_send);

            if (i>0) {
                accum_time += info.Get<double>("elapsed_time");
                accum_mem  += info.Get<std::size_t>("memory_usage_ipc");
                // std::cout << "\n    Elapsed time: " << info.Get<double>("elapsed_time") << std::endl;
                // std::cout << "    Memory usage: " << HumanReadableSize(info.Get<std::size_t>("memory_usage_ipc")) << std::endl;
                // std::cout << "    Speed: " << HumanReadableSize(info.Get<std::size_t>("memory_usage_ipc")/info.Get<double>("elapsed_time")) << "/[s]" << std::endl;
                // if (info.Has("elapsed_time_ipc")) {
                //     std::cout << "    IPC time: " << info.Get<double>("elapsed_time_ipc") << std::endl;
                //     std::cout << "    Serializer time: " << info.Get<double>("elapsed_time_serializer") << std::endl;
                // }
            }
        }

        std::cout << "\n    Elapsed time: " << accum_time/NUM_EVALUATIONS << std::endl;
        std::cout << "    Memory usage: " << HumanReadableSize(accum_mem/NUM_EVALUATIONS) << std::endl;
        std::cout << "    Speed: " << HumanReadableSize(accum_mem/accum_time) << "/[s]" << std::endl;

        CoSimIO::Info disconnect_settings;
        disconnect_settings.Set("connection_name", connection_name);
        info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
        COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);
        std::cout << "\n" << std::endl;
    }

    return 0;
}
