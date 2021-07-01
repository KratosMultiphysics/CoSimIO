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
#include <cmath> // std::abs<double>

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/define.hpp"
#include "impl/stream_serializer.h"


namespace CoSimIO {

namespace {

// dummy class for testing serialization of CoSimIO::intrusive_ptr
class IntrusivelyManaged
{
  public:
    IntrusivelyManaged(double X, int Id) : mX(X), mId(Id) {}

    double GetX() const {return mX;};
    int GetId() const {return mId;};

    bool operator==(const IntrusivelyManaged& rhs) const
    {
        if (std::abs(mX-rhs.mX)>1e-12) {return false;}
        if (mId != rhs.mId) {return false;}
        return true;
    }

  private:
    double mX = 0.0;
    int mId = 0;

    //*********************************************
    //this block is needed for refcounting
    mutable std::atomic<int> mReferenceCounter;

    friend void intrusive_ptr_add_ref(const IntrusivelyManaged* x)
    {
        x->mReferenceCounter.fetch_add(1, std::memory_order_relaxed);
    }

    friend void intrusive_ptr_release(const IntrusivelyManaged* x)
    {
        if (x->mReferenceCounter.fetch_sub(1, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete x;
        }
    }
    //*********************************************

    IntrusivelyManaged()=default; // required for serialization

    friend class CoSimIO::Serializer; // needs "CoSimIO::" because it is in different namespace

    void save(Serializer& rSerializer) const
    {
        rSerializer.save("mX", mX);
        rSerializer.save("mId", mId);
    }

    void load(Serializer& rSerializer)
    {
        rSerializer.load("mX", mX);
        rSerializer.load("mId", mId);
    }
};

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
    SaveAndLoadObjects(rSerializer, rObjectToBeSaved, rObjectToBeLoaded);
    CO_SIM_IO_CHECK_VECTOR_NEAR(rObjectToBeSaved, rObjectToBeLoaded)
}

template<typename TObjectType>
void FillVectorWithValues(TObjectType& rObject)
{
    for (std::size_t i=0; i<rObject.size(); ++i) {
        rObject[i] = i*i*0.2 + 5.333;
    }
}

template<typename TDoublePtrType, typename TVectorPtrType>
void TestBasicSmartPointerSerialization(Serializer& rSerializer)
{
    const std::string tag_string("TestString");
    const std::string tag_string_2("TestString2");

    using DoubleType = typename TDoublePtrType::element_type;
    using VectorType = typename TVectorPtrType::element_type;

    TDoublePtrType p_dbl_save = TDoublePtrType(new DoubleType(-0.25568));
    TVectorPtrType p_vec_save = TVectorPtrType(new VectorType(15));

    FillVectorWithValues(*p_vec_save);

    TDoublePtrType p_dbl_load;
    TVectorPtrType p_vec_load;

    rSerializer.save(tag_string, p_dbl_save);
    rSerializer.save(tag_string_2, p_vec_save);

    rSerializer.load(tag_string, p_dbl_load);
    rSerializer.load(tag_string_2, p_vec_load);

    CHECK_EQ(*p_dbl_save, *p_dbl_load);
    CHECK_NE(p_dbl_save, p_dbl_load);

    CO_SIM_IO_CHECK_VECTOR_NEAR(*p_vec_save, *p_vec_load)
    CHECK_NE(p_vec_save, p_vec_load);
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

    SUBCASE("std::array_int")
    {
        using ArrayType = std::array<int,7>;

        ArrayType object_to_be_saved;
        ArrayType object_to_be_loaded;

        FillVectorWithValues(object_to_be_saved);

        TestObjectSerializationComponentwise1D(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::array_double")
    {
        using ArrayType = std::array<double,12>;

        ArrayType object_to_be_saved;
        ArrayType object_to_be_loaded;

        FillVectorWithValues(object_to_be_saved);

        TestObjectSerializationComponentwise1D(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::vector_int")
    {
        using VectorType = std::vector<int>;

        VectorType object_to_be_saved;
        VectorType object_to_be_loaded;

        FillVectorWithValues(object_to_be_saved);

        TestObjectSerializationComponentwise1D(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::vector_double")
    {
        using VectorType = std::vector<double>;

        VectorType object_to_be_saved(158);
        VectorType object_to_be_loaded(23); // initialized smaller to check if resizing works

        FillVectorWithValues(object_to_be_saved);

        TestObjectSerializationComponentwise1D(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::map")
    {
        std::map <std::string, double> object_to_be_saved {
            { "42", -30.556 },
            { "3",   10.258 }
        };
        std::map<std::string, double> object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::unordered_map")
    {
        std::unordered_map <std::string, double> object_to_be_saved {
            { "42", -30.556 },
            { "3",   10.258 }
        };
        std::unordered_map<std::string, double> object_to_be_loaded;

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::pair")
    {
        std::pair<int, double> object_to_be_saved(42, 0.123);
        std::pair<int, double> object_to_be_loaded(42, 0.123);

        TestObjectSerialization(rSerializer, object_to_be_saved, object_to_be_loaded);
    }

    SUBCASE("std::shared_ptr_basics")
    {
        TestBasicSmartPointerSerialization<std::shared_ptr<double>, std::shared_ptr<std::vector<double>>>(rSerializer);
    }

    SUBCASE("std::unique_ptr_basics")
    {
        TestBasicSmartPointerSerialization<std::unique_ptr<double>, std::unique_ptr<std::vector<double>>>(rSerializer);
    }

    SUBCASE("CoSimIO::intrusive_ptr")
    {
        const std::string tag_string("TestString");

        using PointerType = CoSimIO::intrusive_ptr<IntrusivelyManaged>;

        PointerType p_save = CoSimIO::make_intrusive<IntrusivelyManaged>(-190.87412, 194741);
        PointerType p_load;

        rSerializer.save(tag_string, p_save);
        rSerializer.load(tag_string, p_load);

        CHECK_EQ(*p_save, *p_load);
        CHECK_NE(p_save, p_load);
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
