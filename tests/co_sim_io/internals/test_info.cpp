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

namespace {

void AddLocalInt(Info& rInfo)
{
    // this puts a local variable into the info
    // it should still giv the correct result even after the local var goes out of scope
    int local_var = 15;
    REQUIRE_FALSE(rInfo.Has("local_var_int"));

    rInfo.Set<int>("local_var_int", local_var);
}

}

TEST_CASE("test_info_basics")
{
    Info info;

    REQUIRE_FALSE(info.Has("Some_value_non_existant"));

    info.Set<int>("echo_level", 1);

    REQUIRE(info.Has("echo_level"));

    std::cout << info.Get<int>("echo_level") << std::endl << std::endl;

    REQUIRE(info.Get<int>("echo_level") == 1); // not working
}

TEST_CASE("test_info_local_vars")
{
    Info info;

    AddLocalInt(info);

    REQUIRE(info.Has("local_var_int"));

    REQUIRE(info.Get<int>("local_var_int") == 15); // not working
}

} // namespace CoSimIO
