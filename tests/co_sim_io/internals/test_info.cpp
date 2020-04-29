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

TEST_CASE("info_basics_int")
{
    Info info;

    REQUIRE_FALSE(info.Has("echo_level"));

    info.Set<int>("echo_level", 1);

    REQUIRE(info.Has("echo_level"));

    REQUIRE(info.Get<int>("echo_level") == 1);
}

TEST_CASE("info_basics_double")
{
    Info info;

    REQUIRE_FALSE(info.Has("tolerance"));

    info.Set<double>("tolerance", 1.5);

    REQUIRE(info.Has("tolerance"));

    REQUIRE(info.Get<double>("tolerance") == Approx(1.5));
}

TEST_CASE("info_basics_bool")
{
    Info info;

    REQUIRE_FALSE(info.Has("print_sth"));

    info.Set<bool>("print_sth", false);

    REQUIRE(info.Has("print_sth"));

    REQUIRE(info.Get<bool>("print_sth") == false);
}

TEST_CASE("info_basics_string")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    info.Set<std::string>("identifier", "pressure");

    REQUIRE(info.Has("identifier"));

    REQUIRE(info.Get<std::string>("identifier") == "pressure");
}

TEST_CASE("info_wrong_type")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    info.Set<std::string>("identifier", "pressure");

    REQUIRE(info.Has("identifier"));
    REQUIRE_THROWS_WITH(info.Get<int>("identifier"), "Error: "); // TODO find a better way of testing this
}

TEST_CASE("info_many_values")
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

TEST_CASE("info_set_alreay_existing")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));
    info.Set<std::string>("identifier", "velocity_interface");
    REQUIRE(info.Has("identifier"));
    REQUIRE(info.Get<std::string>("identifier") == "velocity_interface");

    // now overwriting the already existing value
    info.Set<std::string>("identifier", "pressure");
    REQUIRE(info.Get<std::string>("identifier") == "pressure");
}

TEST_CASE("info_set_alreay_existing_different_data_type")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));
    info.Set<std::string>("identifier", "velocity_interface");
    REQUIRE(info.Has("identifier"));
    REQUIRE(info.Get<std::string>("identifier") == "velocity_interface");

    // now overwriting the already existing value with a different type
    info.Set<int>("identifier", 15);
    REQUIRE(info.Get<int>("identifier") == 15);
}

TEST_CASE("info_size")
{
    Info info;
    REQUIRE(info.Size() == 0);

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    REQUIRE(info.Size() == 4);
    info.Set<int>("echo_level", 6);
    REQUIRE(info.Size() == 4);
}

TEST_CASE("info_clear")
{
    Info info;
    REQUIRE(info.Size() == 0);

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    REQUIRE(info.Size() == 4);
    info.Set<int>("echo_level", 6);
    REQUIRE(info.Size() == 4);

    info.Clear();
    REQUIRE(info.Size() == 0);
}

TEST_CASE("info_erase")
{
    Info info;
    REQUIRE(info.Size() == 0);

    info.Set<std::string>("identifier", "velocity_interface");
    REQUIRE(info.Has("identifier"));
    REQUIRE(info.Size() == 1);

    info.Erase("identifier");
    REQUIRE_FALSE(info.Has("identifier"));
    REQUIRE(info.Size() == 0);

    // erasing non-existing keys does not throw
    REQUIRE_NOTHROW(info.Erase("identifier"));
    REQUIRE_NOTHROW(info.Erase("whatever"));
}

TEST_CASE("info_save")
{
    std::cout << std::endl<< std::endl;
    Info info;
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");


    std::stringstream mystream;
    std::ofstream myfstream("ddddd.json");

    mystream << "dbf";

    info.Save(mystream);
    info.Save(myfstream);

    std::cout << mystream.str();

    std::cout << std::endl<< std::endl<< std::endl;
}

TEST_CASE("info_load")
{
    std::cout << std::endl<< std::endl;

    std::stringstream mystream;

    mystream << "3\nkeyword\nInfoData_string\nawesome\nis_converged\nInfoData_bool\n1\n\\tol\nInfoData_double\n1.225";

    Info info;

    info.Load(mystream);


    std::cout << std::endl<< std::endl<< std::endl;
}



} // namespace CoSimIO
