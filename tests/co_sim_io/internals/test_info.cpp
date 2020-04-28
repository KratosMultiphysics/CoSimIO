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

TEST_CASE("test_info_int")
{
    Info info;

    REQUIRE_FALSE(info.Has("echo_level"));

    info.Set<int>("echo_level", 1);

    REQUIRE(info.Has("echo_level"));

    REQUIRE(info.Get<int>("echo_level") == 1);
}

TEST_CASE("test_info_double")
{
    Info info;

    REQUIRE_FALSE(info.Has("tolerance"));

    info.Set<double>("tolerance", 1.5);

    REQUIRE(info.Has("tolerance"));

    REQUIRE(info.Get<double>("tolerance") == Approx(1.5));
}

TEST_CASE("test_info_bool")
{
    Info info;

    REQUIRE_FALSE(info.Has("print_sth"));

    info.Set<bool>("print_sth", false);

    REQUIRE(info.Has("print_sth"));

    REQUIRE(info.Get<bool>("print_sth") == false);
}

TEST_CASE("test_info_string")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    info.Set<std::string>("identifier", "pressure");

    REQUIRE(info.Has("identifier"));

    REQUIRE(info.Get<std::string>("identifier") == "pressure");
}

TEST_CASE("test_info_many_values")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));
    REQUIRE_FALSE(info.Has("is_converged"));
    REQUIRE_FALSE(info.Has("tol"));
    REQUIRE_FALSE(info.Has("echo_level"));

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    REQUIRE(info.Has("identifier"));
    REQUIRE(info.Has("is_converged"));
    REQUIRE(info.Has("tol"));
    REQUIRE(info.Has("echo_level"));

    REQUIRE(info.Get<std::string>("identifier") == "velocity_interface");
    REQUIRE(info.Get<bool>("is_converged") == true);
    REQUIRE(info.Get<double>("tol") == Approx(0.008));
    REQUIRE(info.Get<int>("echo_level") == 2);
}

} // namespace CoSimIO
