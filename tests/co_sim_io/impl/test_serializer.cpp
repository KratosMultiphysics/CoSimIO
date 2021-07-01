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
#include "impl/stream_serializer.h"


namespace CoSimIO {

namespace {

template<typename TObjectType>
void SaveAndLoadObjects(Serializer& rSerializer, const TObjectType& rObjectToBeSaved, TObjectType& rObjectToBeLoaded)
{
    const std::string tag_string("TestString");

    rSerializer.save(tag_string, rObjectToBeSaved);
    rSerializer.load(tag_string, rObjectToBeLoaded);
}

template<typename TObjectType>
void TestObjectSerialization(Serializer& rSerializer, const TObjectType& rObjectToBeSaved, TObjectType& rObjectToBeLoaded)
{
    SaveAndLoadObjects(rSerializer, rObjectToBeSaved, rObjectToBeLoaded);
    CHECK_EQ(rObjectToBeLoaded, rObjectToBeSaved);
}

template<typename TObjectType>
void TestObjectSerializationComponentwise1D(Serializer& rSerializer, const TObjectType& rObjectToBeSaved, TObjectType& rObjectToBeLoaded)
{
    SaveAndLoadObjects(rObjectToBeSaved, rObjectToBeLoaded);

    REQUIRE_EQ(rObjectToBeLoaded.size(), rObjectToBeSaved.size());

    for (std::size_t i=0; i< rObjectToBeSaved.size(); ++i)
        CHECK_EQ(rObjectToBeLoaded[i], rObjectToBeSaved[i]);
}

void RunAllSerializationTests(Serializer& rSerializer)
{
    SUBCASE("bool_true")
    {
        bool object_to_be_saved = true;
        bool object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("bool_false")
    {
        bool object_to_be_saved = false;
        bool object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("int")
    {
        int object_to_be_saved = -18;
        int object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("unsigned int")
    {
        unsigned int object_to_be_saved = 128;
        unsigned int object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("IdType")
    {
        CoSimIO::IdType object_to_be_saved = -19147;
        CoSimIO::IdType object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("float")
    {
        float object_to_be_saved = -128.28547;
        float object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("double")
    {
        double object_to_be_saved = -1555598.6608345;
        double object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("long")
    {
        long object_to_be_saved = -1598456605;
        long object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("long long")
    {
        long long object_to_be_saved = -1598456546843565605;
        long long object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("unsigned long")
    {
        unsigned long object_to_be_saved = 1598456605;
        unsigned long object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::size_t")
    {
        std::size_t object_to_be_saved = 1598456605;
        std::size_t object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::string")
    {
        std::string object_to_be_saved = "EASTEREGG:ThisStringShouldBeSerialized";
        std::string object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }
}

}

TEST_SUITE("Serializer") {

TEST_CASE("Serializer_no_trace")
{
    StreamSerializer serializer(Serializer::TraceType::SERIALIZER_NO_TRACE);
    RunAllSerializationTests(serializer);
}

TEST_CASE("Serializer_trace_error")
{
    StreamSerializer serializer(Serializer::TraceType::SERIALIZER_TRACE_ERROR);
    RunAllSerializationTests(serializer);
}

TEST_CASE("Serializer_trace_all")
{
    StreamSerializer serializer(Serializer::TraceType::SERIALIZER_TRACE_ALL);
    RunAllSerializationTests(serializer);
}

} // TEST_SUITE("Serializer")

} // namespace CoSimIO
