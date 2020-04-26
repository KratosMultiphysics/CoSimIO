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
#include <string>
#include <stdexcept>

// Test includes
#include "interface/interface_tests.hpp"
#include "solver_integration/connect_disconnect/solver.hpp"
#include "solver_integration/data_exchange/solver.hpp"
#include "solver_integration/mesh_exchange/solver.hpp"


int main(int argc, char **argv)
{
    // parsing input
    if (argc != 2) {
        throw std::runtime_error("One input argument required: Name of the test to execute");
    }

    const std::string test_name(argv[1]);

    // run the test with the given test name ...

    return 0;
}