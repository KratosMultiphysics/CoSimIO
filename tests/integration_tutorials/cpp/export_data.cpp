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
#include "ghc/filesystem.hpp"

#include "data_exchange_testing_matrix.hpp"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }


int main()
{
    namespace fs = ghc::filesystem;

    fs::path res_dir("data_exchange_profiling");

    fs::remove_all(res_dir);
    fs::create_directory(res_dir);

    bool header_written = false;
    bool uses_serializer = false;

    int counter = 0;
    for (CoSimIO::Info config : GetTestingMatrix()) {
        header_written = false;
        uses_serializer = false;

        const std::string file_name = GetFileName(config);

        fs::path res_file_name = res_dir / fs::path(file_name);

        std::ofstream res_file;
        res_file.open(res_file_name.string());
        res_file << "# "; // comment for file
        config.Print(res_file, "#");

        std::cout << "Current configuration (" << file_name << "):\n" << config << std::endl;
        const std::string my_name = "cpp_export_solver_" + std::to_string(counter);
        const std::string connect_to = "cpp_import_solver_" + std::to_string(counter);
        counter++;

        config.Set("my_name", my_name);
        config.Set("connect_to", connect_to);
        // config.Set("echo_level", 2);

        auto info = CoSimIO::Connect(config);
        COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Connected);
        const std::string connection_name = info.Get<std::string>("connection_name");

        std::vector<double> data_to_send(VEC_SIZES.back());
        for (std::size_t vec_size : VEC_SIZES) {

            const auto start_time(std::chrono::steady_clock::now());

            data_to_send.resize(vec_size);
            std::cout << "\nCurrent vector size: " << vec_size << std::endl;

            double accum_time = 0.0;
            std::size_t accum_mem = 0;
            double accum_time_ipc = 0.0;
            double accum_time_serializer = 0.0;

            for (int i=0; i<NUM_EVALUATIONS+1; ++i) {
                info.Clear();
                info.Set("identifier", "vector_of_pi_1");
                info.Set("connection_name", connection_name);
                info = CoSimIO::ExportData(info, data_to_send);

                if (i>0) {
                    accum_time += info.Get<double>("elapsed_time");
                    accum_mem  += info.Get<std::size_t>("memory_usage_ipc");
                    if (info.Has("elapsed_time_ipc")) {
                        uses_serializer = true;
                        accum_time_ipc += info.Get<double>("elapsed_time_ipc");
                        accum_time_serializer += info.Get<double>("elapsed_time_serializer");
                    }
                }

                info.Clear();
                info.Set("identifier", "vector_of_pi_2");
                info.Set("connection_name", connection_name);
                info = CoSimIO::ImportData(info, data_to_send);

                if (i>0) {
                    accum_time += info.Get<double>("elapsed_time");
                    accum_mem  += info.Get<std::size_t>("memory_usage_ipc");
                    if (uses_serializer) {
                        accum_time_ipc += info.Get<double>("elapsed_time_ipc");
                        accum_time_serializer += info.Get<double>("elapsed_time_serializer");
                    }
                }
            }

            const double avg_time = accum_time/(NUM_EVALUATIONS*2);
            const double avg_time_ipc = accum_time_ipc/(NUM_EVALUATIONS*2);
            const double avg_time_serializer = accum_time_serializer/(NUM_EVALUATIONS*2);
            const std::size_t avg_mem = accum_mem/(NUM_EVALUATIONS*2);

            std::cout << "    Elapsed time: " << avg_time << std::endl;
            std::cout << "    Memory usage: " << HumanReadableSize(avg_mem) << std::endl;
            std::cout << "    Speed: " << HumanReadableSize(avg_mem/avg_time) << "/s" << std::endl;

            if (!header_written) {
                header_written = true;
                res_file << "\n# Vector-size  avg-time[s]  avg-memory[B]";
                if (uses_serializer) {
                    res_file << " avg-time-ipc[s] avg-time-serializer[s]";
                }
                res_file << "\n";
            }

            res_file << vec_size << " " << avg_time << " " << avg_mem;
            if (uses_serializer) {
                res_file << " " << avg_time_ipc << " " << avg_time_serializer;
            }
            res_file << "\n";

            const bool break_and_go_to_next_config = ElapsedSeconds(start_time) > 2;

            CoSimIO::Info loop_info;
            loop_info.Set("identifier", "loop_info");
            loop_info.Set("connection_name", connection_name);
            loop_info.Set("loop_info", break_and_go_to_next_config);

            CoSimIO::ExportInfo(loop_info);

            if (break_and_go_to_next_config) {
                std::cout << "BREAKING ..." << std::endl;
                break;
            }
        }

        CoSimIO::Info disconnect_settings;
        disconnect_settings.Set("connection_name", connection_name);
        info = CoSimIO::Disconnect(disconnect_settings); // disconnect afterwards
        COSIMIO_CHECK_EQUAL(info.Get<int>("connection_status"), CoSimIO::ConnectionStatus::Disconnected);
        std::cout << "\n" << std::endl;

        res_file.close();
    }

    return 0;
}
