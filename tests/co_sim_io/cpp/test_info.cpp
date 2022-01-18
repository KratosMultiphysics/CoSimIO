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
#include "includes/info.hpp"
#include "includes/stream_serializer.hpp"


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

TEST_CASE("info_basics_size_t")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("echo_level"));

    info.Set<std::size_t>("echo_level", 1);

    CHECK_UNARY(info.Has("echo_level"));

    CHECK_EQ(info.Get<std::size_t>("echo_level"), 1);
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

TEST_CASE("info_basics_info")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    Info sub_info;

    info.Set<Info>("identifier", sub_info);

    CHECK_UNARY(info.Has("identifier"));

    CHECK_EQ(info.Get<Info>("identifier").Size(), 0);
}

TEST_CASE("info_basics_info_nested")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    Info sub_info;

    info.Set<Info>("identifier", sub_info);

    CHECK_UNARY(info.Has("identifier"));

    CHECK_EQ(info.Get<Info>("identifier").Size(), 0);

    sub_info.Set<int>("integer", 13);

    // sub-info is added by copy, changing the original object doesn't have an effect on the sub-info!
    CHECK_EQ(info.Get<Info>("identifier").Size(), 0);
}

TEST_CASE("info_int_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("echo_level"));

    CHECK_EQ(info.Get<int>("echo_level", -3), -3);

    CHECK_UNARY_FALSE(info.Has("echo_level")); // getting the default must not insert it!
}


TEST_CASE("info_size_t_default")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("echo_level"));

    CHECK_EQ(info.Get<std::size_t>("echo_level", 3), 3);

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

TEST_CASE("info_info_default")
{
    Info info;

    Info other_info;
    other_info.Set<int>("integer", 13);

    CHECK_UNARY_FALSE(info.Has("identifier"));

    CHECK_EQ(info.Get<Info>("identifier", other_info).Size(), 1);

    CHECK_UNARY_FALSE(info.Has("identifier")); // getting the default must not insert it!
}

TEST_CASE("info_int_and_size_t")
{
    Info info;

    info.Set("signed", -1);
    info.Set("specunsigned", 1UL);
    info.Set<std::size_t>("unsigned", 1); // this would be an int!

    CHECK_UNARY(info.Has("signed"));
    CHECK_UNARY(info.Has("unsigned"));

    CHECK_EQ(info.Get<int>("signed"), -1);
    CHECK_EQ(info.Get<std::size_t>("specunsigned"), 1);
    CHECK_EQ(info.Get<std::size_t>("unsigned"), 1);
}

TEST_CASE("info_non_existing_key")
{
    Info info;

    CHECK_UNARY_FALSE(info.Has("identifier"));

    CHECK_THROWS_WITH(info.Get<int>("identifier"), "Error: Trying to get \"identifier\" which does not exist!\nCurrently available:\nCoSimIO-Info; containing 0 entries\n\n");
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

TEST_CASE("info_many_values_nested")
{
    Info info;
    Info sub_info;
    Info sub_sub_info;

    CHECK_UNARY_FALSE(info.Has("identifier"));
    CHECK_UNARY_FALSE(info.Has("is_converged"));
    CHECK_UNARY_FALSE(info.Has("tol"));
    CHECK_UNARY_FALSE(info.Has("echo_level"));
    CHECK_UNARY_FALSE(info.Has("counter"));

    info.Set<std::string>("identifier", "velocity_interface");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", -2);
    info.Set<std::size_t>("counter", 2);

    sub_info.Set<std::string>("abc", "something");
    sub_info.Set<bool>("minor", false);
    sub_info.Set<double>("abs", -78.8);
    sub_info.Set<int>("verb", 8);

    sub_sub_info.Set<std::string>("the_key", "diff");
    sub_sub_info.Set<bool>("hello", true);
    sub_sub_info.Set<double>("value", 1.887e4);
    sub_sub_info.Set<int>("id", 8189);

    CHECK_UNARY(info.Has("identifier"));
    CHECK_UNARY(info.Has("is_converged"));
    CHECK_UNARY(info.Has("tol"));
    CHECK_UNARY(info.Has("echo_level"));

    CHECK_UNARY(sub_info.Has("abc"));
    CHECK_UNARY(sub_info.Has("minor"));
    CHECK_UNARY(sub_info.Has("abs"));
    CHECK_UNARY(sub_info.Has("verb"));

    CHECK_UNARY(sub_sub_info.Has("the_key"));
    CHECK_UNARY(sub_sub_info.Has("hello"));
    CHECK_UNARY(sub_sub_info.Has("value"));
    CHECK_UNARY(sub_sub_info.Has("id"));

    sub_info.Set<Info>("sub_sub", sub_sub_info);
    info.Set<Info>("sub", sub_info);

    CHECK_EQ(info.Get<std::string>("identifier"), "velocity_interface");
    CHECK_UNARY(info.Get<bool>("is_converged"));
    CHECK_EQ(info.Get<double>("tol"), doctest::Approx(0.008));
    CHECK_EQ(info.Get<int>("echo_level"), -2);
    CHECK_EQ(info.Get<std::size_t>("counter"), 2);

    Info ret_sub_info = info.Get<Info>("sub");

    CHECK_UNARY(ret_sub_info.Has("abc"));
    CHECK_UNARY(ret_sub_info.Has("minor"));
    CHECK_UNARY(ret_sub_info.Has("abs"));
    CHECK_UNARY(ret_sub_info.Has("verb"));

    CHECK_EQ(ret_sub_info.Get<std::string>("abc"), "something");
    CHECK_UNARY_FALSE(ret_sub_info.Get<bool>("minor"));
    CHECK_EQ(ret_sub_info.Get<double>("abs"), doctest::Approx(-78.8));
    CHECK_EQ(ret_sub_info.Get<int>("verb"), 8);

    Info ret_sub_sub_info = ret_sub_info.Get<Info>("sub_sub");

    CHECK_UNARY(ret_sub_sub_info.Has("the_key"));
    CHECK_UNARY(ret_sub_sub_info.Has("hello"));
    CHECK_UNARY(ret_sub_sub_info.Has("value"));
    CHECK_UNARY(ret_sub_sub_info.Has("id"));

    CHECK_EQ(ret_sub_sub_info.Get<std::string>("the_key"), "diff");
    CHECK_UNARY(ret_sub_sub_info.Get<bool>("hello"));
    CHECK_EQ(ret_sub_sub_info.Get<double>("value"), doctest::Approx(18870.0));
    CHECK_EQ(ret_sub_sub_info.Get<int>("id"), 8189);
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
    info_save.Set<int>("echo_level", -2);
    info_save.Set<int>("checking", 22);
    info_save.Set<std::size_t>("count", 123456789);

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("info", info_save);
    serializer.load("info", info_load);

    CHECK_EQ(info_load.Size(), 6);
    CHECK_EQ(info_load.Get<int>("checking"), 22);
    CHECK_EQ(info_load.Get<int>("echo_level"), -2);
    CHECK_EQ(info_load.Get<std::size_t>("count"), 123456789);
    CHECK_EQ(info_load.Get<std::string>("keyword"), "awesome");
    CHECK_UNARY(info_load.Get<bool>("is_converged"));
    CHECK_EQ(info_load.Get<double>("tol"), doctest::Approx(0.008));
}

TEST_CASE("info_serialization_nested")
{
    Info info_save;
    Info info_load;
    Info sub_info;
    Info sub_sub_info;

    CHECK_UNARY_FALSE(info_save.Has("identifier"));
    CHECK_UNARY_FALSE(info_save.Has("is_converged"));
    CHECK_UNARY_FALSE(info_save.Has("tol"));
    CHECK_UNARY_FALSE(info_save.Has("echo_level"));

    info_save.Set<std::string>("identifier", "velocity_interface");
    info_save.Set<bool>("is_converged", true);
    info_save.Set<double>("tol", 0.008);
    info_save.Set<int>("echo_level", 2);

    sub_info.Set<std::string>("abc", "something");
    sub_info.Set<bool>("minor", false);
    sub_info.Set<double>("abs", -78.8);
    sub_info.Set<int>("verb", 8);

    sub_sub_info.Set<std::string>("the_key", "diff");
    sub_sub_info.Set<bool>("hello", true);
    sub_sub_info.Set<double>("value", 1.887e4);
    sub_sub_info.Set<int>("id", 8189);

    CHECK_UNARY(info_save.Has("identifier"));
    CHECK_UNARY(info_save.Has("is_converged"));
    CHECK_UNARY(info_save.Has("tol"));
    CHECK_UNARY(info_save.Has("echo_level"));

    CHECK_UNARY(sub_info.Has("abc"));
    CHECK_UNARY(sub_info.Has("minor"));
    CHECK_UNARY(sub_info.Has("abs"));
    CHECK_UNARY(sub_info.Has("verb"));

    CHECK_UNARY(sub_sub_info.Has("the_key"));
    CHECK_UNARY(sub_sub_info.Has("hello"));
    CHECK_UNARY(sub_sub_info.Has("value"));
    CHECK_UNARY(sub_sub_info.Has("id"));

    sub_info.Set<Info>("sub_sub", sub_sub_info);
    info_save.Set<Info>("sub", sub_info);

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("info", info_save);
    serializer.load("info", info_load);

    CHECK_EQ(info_load.Get<std::string>("identifier"), "velocity_interface");
    CHECK_UNARY(info_load.Get<bool>("is_converged"));
    CHECK_EQ(info_load.Get<double>("tol"), doctest::Approx(0.008));
    CHECK_EQ(info_load.Get<int>("echo_level"), 2);

    const Info ret_sub_info = info_load.Get<Info>("sub");

    CHECK_UNARY(ret_sub_info.Has("abc"));
    CHECK_UNARY(ret_sub_info.Has("minor"));
    CHECK_UNARY(ret_sub_info.Has("abs"));
    CHECK_UNARY(ret_sub_info.Has("verb"));

    CHECK_EQ(ret_sub_info.Get<std::string>("abc"), "something");
    CHECK_UNARY_FALSE(ret_sub_info.Get<bool>("minor"));
    CHECK_EQ(ret_sub_info.Get<double>("abs"), doctest::Approx(-78.8));
    CHECK_EQ(ret_sub_info.Get<int>("verb"), 8);

    const Info ret_sub_sub_info = ret_sub_info.Get<Info>("sub_sub");

    CHECK_UNARY(ret_sub_sub_info.Has("the_key"));
    CHECK_UNARY(ret_sub_sub_info.Has("hello"));
    CHECK_UNARY(ret_sub_sub_info.Has("value"));
    CHECK_UNARY(ret_sub_sub_info.Has("id"));

    CHECK_EQ(ret_sub_sub_info.Get<std::string>("the_key"), "diff");
    CHECK_UNARY(ret_sub_sub_info.Get<bool>("hello"));
    CHECK_EQ(ret_sub_sub_info.Get<double>("value"), doctest::Approx(18870.0));
    CHECK_EQ(ret_sub_sub_info.Get<int>("id"), 8189);
}

TEST_CASE("info_ostream")
{
    Info info;
    info.Set<std::string>("keyword", "awesome");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", -2);
    info.Set<int>("checking", 22);
    info.Set<std::size_t>("count", 132569748);

    std::stringstream test_stream;

    test_stream << info;

    const std::string exp_string = "CoSimIO-Info; containing 6 entries\n  name: checking | value: 22 | type: int\n  name: count | value: 132569748 | type: size_t\n  name: echo_level | value: -2 | type: int\n  name: is_converged | value: true | type: bool\n  name: keyword | value: awesome | type: string\n  name: tol | value: 0.008 | type: double\n";

    CHECK_EQ(test_stream.str(), exp_string);
}

TEST_CASE("info_ostream_nested")
{
    Info info;
    Info sub_info;
    Info sub_sub_info;
    info.Set<std::string>("keyword", "awesome");
    info.Set<bool>("is_converged", true);
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    sub_info.Set<std::string>("abc", "something");
    sub_info.Set<bool>("minor", false);
    sub_info.Set<double>("abs", -78.8);
    sub_info.Set<int>("verb", 8);

    sub_sub_info.Set<bool>("hello", true);
    sub_sub_info.Set<int>("id", 8189);

    sub_info.Set<Info>("sub_sub", sub_sub_info);
    info.Set<Info>("sub", sub_info);

    std::stringstream test_stream;

    test_stream << info;

    const std::string exp_string =
R"(CoSimIO-Info; containing 6 entries
  name: checking | value: 22 | type: int
  name: echo_level | value: 2 | type: int
  name: is_converged | value: true | type: bool
  name: keyword | value: awesome | type: string
  name: sub | type: CoSimIO-Info; containing 5 entries
    name: abc | value: something | type: string
    name: abs | value: -78.8 | type: double
    name: minor | value: false | type: bool
    name: sub_sub | type: CoSimIO-Info; containing 2 entries
      name: hello | value: true | type: bool
      name: id | value: 8189 | type: int
    name: verb | value: 8 | type: int
  name: tol | value: 0.008 | type: double
)";

    CHECK_EQ(test_stream.str(), exp_string);
}

TEST_CASE("info_copy_constructor")
{
    Info info;
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", -2);
    info.Set<int>("checking", 22);
    info.Set<std::size_t>("count", 12584422);

    Info another_info(info);

    CHECK_EQ(another_info.Size(), 6);
    CHECK_EQ(another_info.Get<int>("checking"), 22);
    CHECK_EQ(another_info.Get<std::size_t>("count"), 12584422);
    CHECK_EQ(another_info.Get<int>("echo_level"), -2);
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
    info.Set<int>("echo_level", -2);
    info.Set<int>("checking", 22);
    info.Set<std::size_t>("count", 12584422);

    Info another_info;
    another_info = info;

    CHECK_EQ(another_info.Size(), 6);
    CHECK_EQ(another_info.Get<int>("checking"), 22);
    CHECK_EQ(another_info.Get<std::size_t>("count"), 12584422);
    CHECK_EQ(another_info.Get<int>("echo_level"), -2);
    CHECK_EQ(another_info.Get<std::string>("keyword"), "awesome");
    CHECK_UNARY(another_info.Get<bool>("is_converged"));
    CHECK_EQ(another_info.Get<double>("tol"), doctest::Approx(0.008));
}

} // TEST_SUITE("Info")

} // namespace CoSimIO
