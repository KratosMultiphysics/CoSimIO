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

// Project includes
#include "includes/code_location.hpp"
#ifndef _WIN32
    #include "includes/filesystem_inc.hpp"
#endif

namespace CoSimIO {
namespace Internals {

std::string CodeLocation::GetCleanFileName() const
{
#ifndef _WIN32
    return fs::canonical(fs::path(GetFileName())).lexically_relative(fs::absolute(".")).string();
#else
    // NOTE: This is a workaround for the above, since it doesn't work on some Windows systems. Inspired in Kratos solution https://github.com/KratosMultiphysics/Kratos/blob/c1d9b2e30b557612aa379acfa2e16298523731a8/kratos/sources/code_location.cpp#L46
    std::string clean_file_name(GetFileName());

    // Replace all backslashes with forward slashes
    std::size_t start_position = 0;
    const std::string from_string = "\\";
    const std::string to_string = "/";
    while ((start_position = clean_file_name.find(from_string, start_position)) != std::string::npos) {
        clean_file_name.replace(start_position, from_string.length(), to_string);
        start_position += to_string.length();
    }

    // Remove the path up to the co_sim_io root folder
    std::size_t co_sim_io_root_position = clean_file_name.rfind("/co_sim_io/");
    if (co_sim_io_root_position != std::string::npos) {
        clean_file_name.erase(0, co_sim_io_root_position+1);
        return clean_file_name;
    }
    if (co_sim_io_root_position != std::string::npos)
        clean_file_name.erase(0, co_sim_io_root_position + 1 );

    // Return the file name
    return clean_file_name;
#endif
}

} // namespace Internals
} // namespace CoSimIO
