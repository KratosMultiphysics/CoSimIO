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
#include <cstdlib>

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/define.hpp"
#include "impl/data_container.hpp"
#include "impl/stream_serializer.hpp"


namespace CoSimIO {
namespace Internals { // DataContainer is in the Internals namespace

using DataContainerBase = DataContainer<double>;
using DataContainerBasePointer = std::unique_ptr<DataContainerBase>;
using DataContainerStdVectorType = DataContainerStdVector<double>;
using DataContainerRawMemoryType = DataContainerRawMemory<double>;
using DataContainerStdVectorReadOnlyType = DataContainerStdVectorReadOnly<double>;
using DataContainerRawMemoryReadOnlyType = DataContainerRawMemoryReadOnly<double>;

namespace {

void TestDataContainerDifferentValues(const std::vector<std::vector<double>>& rRefValues, DataContainerBase& rDataContainer)
{
    std::vector<double> orig_values;

    for (const auto& r_current_ref_vals : rRefValues) {
        const std::size_t current_size(r_current_ref_vals.size());

        // saving current values to make sure they are preserved after resize
        const std::size_t orig_size(rDataContainer.size());
        orig_values.clear();
        orig_values.resize(orig_size);
        for (std::size_t i=0; i<orig_size; ++i) {
            orig_values[i] = rDataContainer[i];
        }

        // resizing if necessary
        if (rDataContainer.size() != current_size) {
            rDataContainer.resize(current_size);
        }

        CO_SIM_IO_CHECK_VECTOR_NEAR_SIZE(orig_values, rDataContainer, std::min(orig_size, current_size))

        // setting new values
        for (std::size_t i=0; i<current_size; ++i) {
            rDataContainer[i] = r_current_ref_vals[i];
        }

        // checking size
        CHECK_EQ(current_size, rDataContainer.size());

        // checking values
        CO_SIM_IO_CHECK_VECTOR_NEAR(r_current_ref_vals, rDataContainer)
    }
}

} // empty namespace

TEST_SUITE("DataContainer")
{

TEST_CASE("DataContainer_StdVector_basics")
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    std::vector<double> values_vec(ref_values);

    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerStdVectorType>(values_vec));

    CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, (*p_container));
}

TEST_CASE("DataContainer_StdVectorReadOnly_basics")
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    const DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerStdVectorReadOnlyType>(ref_values));

    const DataContainerBase& const_ref = *p_container;

    CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, const_ref);
}

TEST_CASE("DataContainer_StdVector_multiple_resizes")
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5},
        {-88.66, 77.9}
    };

    std::vector<double> values_vec;

    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerStdVectorType>(values_vec));

    TestDataContainerDifferentValues(ref_values, *p_container);
}

TEST_CASE("DataContainer_RawMemory_empty")
{
    double* data;

    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(&data, 0));

    CHECK_EQ(0, p_container->size());
}

TEST_CASE("DataContainer_RawMemory_empty_resize")
{
    double* data;

    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(&data, 0));

    CHECK_EQ(0, p_container->size());

    p_container->resize(2);

    CHECK_EQ(2, p_container->size());

    // deallocating memory
    free(data);
}

TEST_CASE("DataContainer_RawMemory_basics")
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    const std::size_t cur_size(ref_values.size());

    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0]= (double*)malloc(sizeof(double)*cur_size);

    for (std::size_t i=0; i<cur_size; ++i) {
        (*values_raw)[i] = ref_values[i];
    }

    const DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(values_raw, cur_size));

    CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, (*p_container));

    // deallocating memory
    free(*values_raw);
    free(values_raw);
}

TEST_CASE("DataContainer_RawMemoryReadOnly_basics")
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    const std::size_t cur_size(ref_values.size());

    SUBCASE("malloc")
    {
        double** values_raw = (double**)malloc(sizeof(double*)*1);
        values_raw[0]= (double*)malloc(sizeof(double)*cur_size);

        for (std::size_t i=0; i<cur_size; ++i) {
            (*values_raw)[i] = ref_values[i];
        }

        const DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryReadOnlyType>(*values_raw, cur_size));

        const DataContainerBase& const_ref = *p_container;

        CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, const_ref);

        // deallocating memory
        free(*values_raw);
        free(values_raw);
    }

    SUBCASE("array")
    {
        double values[] = {1.0, -2.333, 15.88, 14.7, -99.6};
        const DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryReadOnlyType>(&(*values), cur_size));

        const DataContainerBase& const_ref = *p_container;
        CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, const_ref);
    }
}

TEST_CASE("DataContainer_RawMemory_resize_larger")
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5}
    };

    double* data;
    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(&data, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(data);
}

TEST_CASE("DataContainer_RawMemory_resize_smaller")
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9}
    };

    double* data;
    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(&data, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(data);
}

TEST_CASE("DataContainer_RawMemory_multiple_resizes")
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9},
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5},
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9}
    };

    double* data;
    DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerRawMemoryType>(&data, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(data);
}

TEST_CASE("DataContainer_serialization_StdVector")
{
    std::vector<double> save_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    std::vector<double> load_values;

    DataContainerBasePointer p_save_container;

    SUBCASE("mutable")
    {
        p_save_container = CoSimIO::make_unique<DataContainerStdVector<double>>(save_values);
    }
    SUBCASE("readonly")
    {
        p_save_container = CoSimIO::make_unique<DataContainerStdVectorReadOnly<double>>(save_values);
    }

    const DataContainerBase& const_ref_save = *p_save_container;

    DataContainerBasePointer p_load_container(CoSimIO::make_unique<DataContainerStdVector<double>>(load_values));
    DataContainerBase& ref_load = *p_load_container;

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("container", const_ref_save);
    serializer.load("container", ref_load);

    CO_SIM_IO_CHECK_VECTOR_NEAR(save_values, load_values);
    CO_SIM_IO_CHECK_VECTOR_NEAR(const_ref_save, ref_load);
}

} // TEST_SUITE("DataContainer")

} // namespace Internals
} // namespace CoSimIO
