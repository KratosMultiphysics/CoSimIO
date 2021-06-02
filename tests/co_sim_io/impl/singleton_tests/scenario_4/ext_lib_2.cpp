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

// Project includes
#include "co_sim_io.hpp"
#include "ext_lib_2.hpp"

#include <iostream>

bool ExtLib2HasIO(const std::string& rConnectionName)
{
    std::cout << "ExtLib2HasIO: " << std::boolalpha << CoSimIO::Internals::HasIO(rConnectionName) << std::endl;

    return CoSimIO::Internals::HasIO(rConnectionName);
}
