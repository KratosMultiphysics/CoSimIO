
#pragma once

#include "co_sim_io.hpp"
#include <cmath>
#include <chrono>

constexpr int NUM_EVALUATIONS = 20;

std::vector<CoSimIO::Info> GetTestingMatrix()
{
    std::vector<CoSimIO::Info> configs;
    CoSimIO::Info info;

    std::vector<std::string> comm_methods {
        "socket",
        "file"
    };

    // adding basic configs
    for (const auto& r_method : comm_methods) {
        info.Clear();

        info.Set("communication_format", r_method);
        configs.push_back(info);
    }

    std::cout << "Number of configurations: " << configs.size() << std::endl;

    return configs;
}

std::string GetFileName(const CoSimIO::Info& rInfo)
{
    const std::string comm_format = rInfo.Get<std::string>("communication_format");
    std::string file_name = comm_format;

    if (rInfo.Has("always_use_serializer")) {
        const bool always_use_serializer = rInfo.Get<bool>("always_use_serializer");
        if (always_use_serializer) {
            file_name += "_serialized";
        }

        if (rInfo.Has("serializer_trace_type")) {
            file_name += "_" + rInfo.Get<std::string>("serializer_trace_type");
        }
    }

    if (comm_format == "file" && rInfo.Has("use_file_serializer")) {
        const bool use_file_serializer = rInfo.Get<bool>("use_file_serializer");
        if (!use_file_serializer) {
            file_name += "_stream_ser";
        }
    }

    // if (comm_format == "pipe" && rInfo.Has("buffer_size")) {
    //     file_name += "_buf_" + std::to_string(rInfo.Get<int>("buffer_size"));
    // }

    return file_name+".dat";
}



std::vector<std::size_t> VEC_SIZES {
    static_cast<std::size_t>(1e0)
    ,static_cast<std::size_t>(1e1)
    ,static_cast<std::size_t>(1e2)
    ,static_cast<std::size_t>(1e3)
    ,static_cast<std::size_t>(1e4)
    ,static_cast<std::size_t>(1e5)
    ,static_cast<std::size_t>(1e6)
    ,static_cast<std::size_t>(1e7)
    ,static_cast<std::size_t>(1e8)
    //,static_cast<std::size_t>(2e8)
};


double ElapsedSeconds(const std::chrono::steady_clock::time_point& rStartTime)
{
    using namespace std::chrono;
    return duration_cast<duration<double>>(steady_clock::now() - rStartTime).count();
}

std::string HumanReadableSize(std::size_t InBytes) {
    constexpr char extension[] = {'\0', 'K', 'M', 'G', 'T', 'P', 'E', 'E'};

    std::stringstream output;
    output.precision(4);

    double output_size = static_cast<double>(InBytes);
    int i = 0;
    for (; i < 7; i++) {
      if (output_size < 1024) {
          break;
      }
      output_size /= 1024;
    }
    output << output_size << " " << extension[i] << 'B';
    return output.str();
}