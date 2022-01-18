
#pragma once

#include "co_sim_io.hpp"

constexpr int NUM_EVALUATIONS = 20;

std::vector<CoSimIO::Info> GetTestingMatrix()
{
    std::vector<CoSimIO::Info> configs;

    CoSimIO::Info info;
    info.Set("communication_format", "file");
    configs.push_back(info);

    info.Clear();
    info.Set("communication_format", "pipe");
    configs.push_back(info);

    info.Clear();
    info.Set("communication_format", "socket");
    configs.push_back(info);

    info.Clear();
    info.Set("communication_format", "local_socket");
    configs.push_back(info);

    info.Clear();
    info.Set("communication_format", "pipe");
    info.Set("buffer_size", 1048576);
    configs.push_back(info);

    info.Clear();
    info.Set("communication_format", "socket");
    info.Set("ip_address", "129.187.141.209");
    configs.push_back(info);






    return configs;
}


std::string HumanReadableSize(std::size_t InBytes) {
    constexpr char extension[] = {'\0', 'K', 'M', 'G', 'T', 'P', 'E', 'E'};

    std::stringstream output;
    output.precision(4);

    double output_size = InBytes;
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