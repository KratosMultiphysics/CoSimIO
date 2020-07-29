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


namespace CoSimIO {

TEST_SUITE("Info") {

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

    REQUIRE(info.Get<double>("tolerance") == doctest::Approx(1.5));
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

TEST_CASE("info_basics_char")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    std::string val = "pressure";

    info.Set<std::string>("identifier", val.c_str());

    REQUIRE(info.Has("identifier"));

    REQUIRE(info.Get<std::string>("identifier") == "pressure"); // char* will be turned into string
}

TEST_CASE("info_int_default")
{
    Info info;

    REQUIRE_FALSE(info.Has("echo_level"));

    REQUIRE(info.Get<int>("echo_level", 3) == 3);

    REQUIRE_FALSE(info.Has("echo_level")); // getting the default must not insert it!
}

TEST_CASE("info_double_default")
{
    Info info;

    REQUIRE_FALSE(info.Has("tolerance"));

    REQUIRE(info.Get<double>("tolerance", 1.5) == doctest::Approx(1.5));

    REQUIRE_FALSE(info.Has("tolerance")); // getting the default must not insert it!
}

TEST_CASE("info_bool_default")
{
    Info info;

    REQUIRE_FALSE(info.Has("is_converged"));

    REQUIRE(info.Get<bool>("is_converged", true));
    REQUIRE_FALSE(info.Get<bool>("is_converged", false));

    REQUIRE_FALSE(info.Has("is_converged")); // getting the default must not insert it!
}

TEST_CASE("info_string_default")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    REQUIRE(info.Get<std::string>("identifier", "dummy") == "dummy");

    REQUIRE_FALSE(info.Has("identifier")); // getting the default must not insert it!
}

TEST_CASE("info_non_existing_key")
{
    Info info;

    REQUIRE_FALSE(info.Has("identifier"));

    REQUIRE_THROWS_WITH(info.Get<int>("identifier"), "Error: "); // TODO find a better way of testing this
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
    REQUIRE(info.Get<double>("tol") == doctest::Approx(0.008));
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
    Info info;
    info.Set<std::string>("keyword", "awesome");
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    std::stringstream test_stream;

    info.Save(test_stream);

    const std::string exp_string = "5\nchecking\nInfoData_int\n22\necho_level\nInfoData_int\n2\nis_converged\nInfoData_bool\n1\nkeyword\nInfoData_string\nawesome\ntol\nInfoData_double\n0.008\n";

    REQUIRE(test_stream.str() == exp_string);
}

TEST_CASE("info_load")
{
    std::stringstream test_stream;

    test_stream << "5\nkeyword\nInfoData_string\nawesome\nis_converged\nInfoData_bool\n1\ntol\nInfoData_double\n1.225\necho_level\nInfoData_int\n2\nchecking\nInfoData_int\n22\n";

    Info info;
    info.Load(test_stream);

    REQUIRE(info.Size() == 5);
    REQUIRE(info.Get<int>("checking") == 22);
    REQUIRE(info.Get<int>("echo_level") == 2);
    REQUIRE(info.Get<std::string>("keyword") == "awesome");
    REQUIRE(info.Get<bool>("is_converged") == true);
    REQUIRE(info.Get<double>("tol") == doctest::Approx(1.225));
}

TEST_CASE("info_save_load")
{
    Info info;
    info.Set<bool>("is_converged", true);
    info.Set<std::string>("keyword", "awesome");
    info.Set<double>("tol", 0.008);
    info.Set<int>("echo_level", 2);
    info.Set<int>("checking", 22);

    std::stringstream test_stream;

    info.Save(test_stream);

    Info another_info;
    another_info.Load(test_stream);

    REQUIRE(another_info.Size() == 5);
    REQUIRE(another_info.Get<int>("checking") == 22);
    REQUIRE(another_info.Get<int>("echo_level") == 2);
    REQUIRE(another_info.Get<std::string>("keyword") == "awesome");
    REQUIRE(another_info.Get<bool>("is_converged") == true);
    REQUIRE(another_info.Get<double>("tol") == doctest::Approx(0.008));
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

    REQUIRE(test_stream.str() == exp_string);
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

    REQUIRE(another_info.Size() == 5);
    REQUIRE(another_info.Get<int>("checking") == 22);
    REQUIRE(another_info.Get<int>("echo_level") == 2);
    REQUIRE(another_info.Get<std::string>("keyword") == "awesome");
    REQUIRE(another_info.Get<bool>("is_converged") == true);
    REQUIRE(another_info.Get<double>("tol") == doctest::Approx(0.008));
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

    REQUIRE(another_info.Size() == 5);
    REQUIRE(another_info.Get<int>("checking") == 22);
    REQUIRE(another_info.Get<int>("echo_level") == 2);
    REQUIRE(another_info.Get<std::string>("keyword") == "awesome");
    REQUIRE(another_info.Get<bool>("is_converged") == true);
    REQUIRE(another_info.Get<double>("tol") == doctest::Approx(0.008));
}

} // TEST_SUITE("Info")

} // namespace CoSimIO
