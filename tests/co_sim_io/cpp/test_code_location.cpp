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
#include <sstream>
#include <string>

// Project includes
#include "co_sim_io_testing.hpp"
#include "includes/code_location.hpp"


namespace testing_aux_namespace {
    CoSimIO::Internals::CodeLocation AuxFunction()
    {
        return CO_SIM_IO_CODE_LOCATION;
    }
}

namespace CoSimIO {

TEST_SUITE("CodeLocation") {

TEST_CASE("macro_CO_SIM_IO_CODE_LOCATION")
{
    auto code_loc = testing_aux_namespace::AuxFunction();
    CHECK_MESSAGE(code_loc.GetCleanFileName().find("co_sim_io/cpp/test_code_location.cpp") != std::string::npos, code_loc.GetCleanFileName()); // must contain at least this
    CHECK_EQ(code_loc.GetLineNumber(), 25);
    CHECK_MESSAGE(code_loc.GetCleanFunctionName().find("testing_aux_namespace::AuxFunction()") != std::string::npos, code_loc.GetCleanFunctionName()); // must contain at least this
}

} // TEST_SUITE("CodeLocation")

} // namespace CoSimIO
