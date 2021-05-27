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
#include <iostream>

// Project includes
// #define CO_SIM_IO_EXTERN
#include "co_sim_io.hpp"

bool CheckSub(const std::string& ConnectionName)
{
    std::cout << "\nHas_io: "<< CoSimIO::Internals::HasIO(ConnectionName) << std::endl<< std::endl;



    return true;

}