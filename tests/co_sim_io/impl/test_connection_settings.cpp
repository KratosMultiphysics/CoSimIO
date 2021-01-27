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

// Project includes
#include "co_sim_io_testing.hpp"
#include "co_sim_io.hpp"


namespace CoSimIO {

TEST_SUITE("ConnectionSettings") {

TEST_CASE("connection_empty_name")
{
    Info info;
    info.Set<std::string>("my_name", "");
    info.Set<std::string>("connect_to", "abc");

    CHECK_THROWS_WITH(CoSimIO::Connect(info), "Error: Using an empty entry for \"my_name\" is not allowed!\n");

    info.Set<std::string>("my_name", "def");
    info.Set<std::string>("connect_to", "");

    CHECK_THROWS_WITH(CoSimIO::Connect(info), "Error: Using an empty entry for \"connect_to\" is not allowed!\n");
}

TEST_CASE("too_long_name")
{
    Info info;
    std::string long_string;
    long_string.resize(1010);

    info.Set<std::string>("my_name", long_string);
    info.Set<std::string>("connect_to", "abc");

    CHECK_THROWS_WITH(CoSimIO::Connect(info), "Error: Entry for \"my_name\" is too long! Maximum allowed length: 1000 characters!\n");

    info.Set<std::string>("my_name", "def");
    info.Set<std::string>("connect_to", long_string);

    CHECK_THROWS_WITH(CoSimIO::Connect(info), "Error: Entry for \"connect_to\" is too long! Maximum allowed length: 1000 characters!\n");
}

TEST_CASE("disallowed_chars")
{
    const char disallowed_chars[] = {'.', ',', ':', ';', '>', '<', '/', '\'', '|', '*', '!', '"', ' '};

    Info info;

    info.Set<std::string>("connect_to", "abc");
    for (const auto ch : disallowed_chars) {
        info.Set<std::string>("my_name", "abc"+std::string(1,ch)+"def");
        std::string exp_err_msg("Error: Entry for \"my_name\" contains a character that is not allowed: \"" + std::string(1,ch) +  "\"!\n");
        CHECK_THROWS_WITH(CoSimIO::Connect(info), exp_err_msg.c_str());
    }

    info.Set<std::string>("my_name", "def");
    for (const auto ch : disallowed_chars) {
        info.Set<std::string>("connect_to", "abc"+std::string(1,ch)+"def");
        std::string exp_err_msg("Error: Entry for \"connect_to\" contains a character that is not allowed: \"" + std::string(1,ch) +  "\"!\n");
        CHECK_THROWS_WITH(CoSimIO::Connect(info), exp_err_msg.c_str());
    }
}

} // TEST_SUITE("ConnectionSettings")

} // namespace CoSimIO
