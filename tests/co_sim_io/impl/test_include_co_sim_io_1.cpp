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

/* this file makes sure that the CoSimIO can be safely included in different cpp files
i.e. it ensures that all the functions are inlined otherwise linking errors occur
the "other" file that includes CoSimIO is "test_include_co_sim_io_2.cpp"
*/

// Project includes
#define CO_SIM_IO_EXTERN
#include "co_sim_io.hpp"
