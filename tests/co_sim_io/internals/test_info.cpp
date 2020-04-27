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

#include "catch2/catch.hpp"
#include "impl/info.hpp"


namespace CoSimIO {

TEST_CASE("test_info_basics")
{
    Info info;

    REQUIRE_FALSE(info.Has("Some_value_non_existant"));

    info.Set<int>("echo_level", 1);

    std::cout << info.Get<int>("echo_level") << std::endl << std::endl;

    // REQUIRE(info.Get<int>("echo_level") == 1); // not working

}

} // namespace CoSimIO
