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

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/info.hpp"
#include "impl/stream_serializer.hpp"


namespace CoSimIO {

TEST_SUITE("Info") {

TEST_CASE("info_basics_int")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("echo_level"));

    info.Set<int>("echo_level", 1);

    CHECK_UNARY(info.Has("echo_level"));

    CHECK_EQ(info.Get<int>("echo_level"), 1);
}

TEST_CASE("info_basics_double")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("tolerance"));

    info.Set<double>("tolerance", 1.5);

    CHECK_UNARY(info.Has("tolerance"));

    CHECK_EQ(info.Get<double>("tolerance"), doctest::Approx(1.5));
}

TEST_CASE("info_basics_bool")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("print_sth"));

    info.Set<bool>("print_sth", false);

    CHECK_UNARY(info.Has("print_sth"));

    CHECK_UNARY_FALSE(info.Get<bool>("print_sth"));
}

TEST_CASE("info_basics_string")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    info.Set<std::string>("identifier", "pressure");

    CHECK_UNARY(info.Has("identifier"));

    CHECK_EQ(info.Get<std::string>("identifier"), "pressure");
}

TEST_CASE("info_basics_char")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    std::string val = "pressure";

    info.Set<std::string>("identifier", val.c_str());

    CHECK_UNARY(info.Has("identifier"));

    CHECK_EQ(info.Get<std::string>("identifier"), "pressure"); // char* will be turned into string
}

TEST_CASE("info_int_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("echo_level"));

    CHECK_EQ(info.Get<int>("echo_level", 3), 3);

    CHECK_UNARY_FALSE(info.Has("echo_level")); // getting the default must not insert it!
}

TEST_CASE("info_double_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("tolerance"));

    CHECK_EQ(info.Get<double>("tolerance", 1.5), doctest::Approx(1.5));

    CHECK_UNARY_FALSE(info.Has("tolerance")); // getting the default must not insert it!
}

TEST_CASE("info_bool_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("is_converged"));

    CHECK_UNARY(info.Get<bool>("is_converged", true));
    CHECK_UNARY_FALSE(info.Get<bool>("is_converged", false));

    CHECK_UNARY_FALSE(info.Has("is_converged")); // getting the default must not insert it!
}

TEST_CASE("info_string_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    CHECK_EQ(info.Get<std::string>("identifier", "dummy"), "dummy");

    CHECK_UNARY_FALSE(info.Has("identifier")); // getting the default must not insert it!
}

TEST_CASE("info_non_existing_key")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    CHECK_THROWS_WITH(info.Get<int>("identifier"), "Error: Trying to get \"identifier\" which does not exist!\n");
}

TEST_CASE("info_wrong_type")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    info.Set<std::string>("identifier", "pressure");

    CHECK_UNARY(info.Has("identifier"));
    CHECK_THROWS_WITH(info.Get<int>("identifier"), "Error: Wrong DataType! Trying to get \"identifier\" which is of type \"string\" with \"int\"!\n");
}

TEST_CASE("info_many_values")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));
    CHECK_UNARY_FALSE(info.Has("is_converged"));
    CHECK_UNARY_FALSE(info.Has("tol"));
    CHECK_UNARY_FALSE(info.Has("echo_level"));

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    CHECK_UNARY(info.Has("identifier"));
    CHECK_UNARY(info.Has("is_converged"));
    CHECK_UNARY(info.Has("tol"));
    CHECK_UNARY(info.Has("echo_level"));

    CHECK_EQ(info.Get<std::string>("identifier"), "velocity_interface");
    CHECK_UNARY(info.Get<bool>("is_converged"));
    CHECK_EQ(info.Get<double>("tol"), doctest::Approx(0.008));
    CHECK_EQ(info.Get<int>("echo_level"), 2);
}

TEST_CASE("info_set_alreay_existing")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));
    info.Set<std::string>("identifier", "velocity_interface");
    CHECK_UNARY(info.Has("identifier"));
    CHECK_EQ(info.Get<std::string>("identifier"), "velocity_interface");

    // now overwriting the already existing value
    info.Set<std::string>("identifier", "pressure");
    CHECK_EQ(info.Get<std::string>("identifier"), "pressure");
}

TEST_CASE("info_set_alreay_existing_different_data_type")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));
    info.Set<std::string>("identifier", "velocity_interface");
    CHECK_UNARY(info.Has("identifier"));
    CHECK_EQ(info.Get<std::string>("identifier"), "velocity_interface");

    // now overwriting the already existing value with a different type
    info.Set<int>("identifier", 15);
    CHECK_EQ(info.Get<int>("identifier"), 15);
}

TEST_CASE("info_size")
{
    Info info;
    CHECK_EQ(info.Size(), 0);

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    CHECK_EQ(info.Size(), 4);
    info.Set<int>("echo_level", 6);
    CHECK_EQ(info.Size(), 4);
}

TEST_CASE("info_clear")
{
    Info info;
    CHECK_EQ(info.Size(), 0);

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);

    CHECK_EQ(info.Size(), 4);
    info.Set<int>("echo_level", 6);
    CHECK_EQ(info.Size(), 4);

    info.Clear();
    CHECK_EQ(info.Size(), 0);
}

TEST_CASE("info_erase")
{
    Info info;
    CHECK_EQ(info.Size(), 0);

    info.Set<std::string>("identifier", "velocity_interface");
    CHECK_UNARY(info.Has("identifier"));
    CHECK_EQ(info.Size(), 1);

    info.Erase("identifier");
    CHECK_UNARY_FALSE(info.Has("identifier"));
    CHECK_EQ(info.Size(), 0);

    // erasing non-existing keys does not throw
    CHECK_NOTHROW(info.Erase("identifier"));
    CHECK_NOTHROW(info.Erase("whatever"));
}

TEST_CASE("info_serialization")
{
    Info info_save;
    Info info_load;
    info_save.Set<bool>("is_converged", true);
    info_save.Set<std::string>("keyword", "awesome");
    info_save.Set<double>("tol", 0.008);
    info_save.Set<int>("echo_level", 2);
    info_save.Set<int>("checking", 22);

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("info", info_save);
    serializer.load("info", info_load);

    CHECK_EQ(info_load.Size(), 5);
    CHECK_EQ(info_load.Get<int>("checking"), 22);
    CHECK_EQ(info_load.Get<int>("echo_level"), 2);
    CHECK_EQ(info_load.Get<std::string>("keyword"), "awesome");
    CHECK_UNARY(info_load.Get<bool>("is_converged"));
    CHECK_EQ(info_load.Get<double>("tol"), doctest::Approx(0.008));
}

TEST_CASE("info_ostream")
{
    Info info;
    info.Set<std::string>("keyword", "awesome");
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    std::stringstream test_stream;

    test_stream << info;

    const std::string exp_string = "CoSimIO-Info; containing 5 entries\n  name: checking | value: 22 | type: int\n  name: echo_level | value: 2 | type: int\n  name: is_converged | value: 1 | type: bool\n  name: keyword | value: awesome | type: string\n  name: tol | value: 0.008 | type: double\n";

    CHECK_EQ(test_stream.str(), exp_string);
}

TEST_CASE("info_copy_constructor")
{
    Info info;
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    Info another_info(info);

    CHECK_EQ(another_info.Size(), 5);
    CHECK_EQ(another_info.Get<int>("checking"), 22);
    CHECK_EQ(another_info.Get<int>("echo_level"), 2);
    CHECK_EQ(another_info.Get<std::string>("keyword"), "awesome");
    CHECK_UNARY(another_info.Get<bool>("is_converged"));
    CHECK_EQ(another_info.Get<double>("tol"), doctest::Approx(0.008));

    // make sure it is a deep copy and the original info hasn't changed
    another_info.Set<std::string>("keyword", "whatever");
    CHECK_EQ(info.Get<std::string>("keyword"), "awesome");
}

TEST_CASE("info_assignment")
{
    Info info;
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    Info another_info;
    another_info = info;

    CHECK_EQ(another_info.Size(), 5);
    CHECK_EQ(another_info.Get<int>("checking"), 22);
    CHECK_EQ(another_info.Get<int>("echo_level"), 2);
    CHECK_EQ(another_info.Get<std::string>("keyword"), "awesome");
    CHECK_UNARY(another_info.Get<bool>("is_converged"));
    CHECK_EQ(another_info.Get<double>("tol"), doctest::Approx(0.008));
}

} // TEST_SUITE("Info")

} // namespace CoSimIO
