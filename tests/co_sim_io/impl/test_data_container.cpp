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
#include <memory>

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/data_container.hpp"


namespace CoSimIO {
namespace Internals { // DataContainer is in the Internals namespace
namespace Testing {

using DataContainerBase = DataContainer<double>;
using DataContainerBasePointer = std::unique_ptr<DataContainerBase>;
using DataContainerStdVectorType = DataContainerStdVector<double>;
using DataContainerRawMemoryType = DataContainerRawMemory<double>;
using DataContainerStdVectorReadOnlyType = DataContainerStdVectorReadOnly<double>;
using DataContainerRawMemoryReadOnlyType = DataContainerRawMemoryReadOnly<double>;

TEST_SUITE("DataContainer")
{

TEST_CASE("DataContainerStdVector_basics")
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    std::vector<double> values_vec(ref_values);

    DataContainerBasePointer p_container;

    p_container = CoSimIO::make_unique<DataContainerStdVectorType>(values_vec);

    CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, (*p_container));
}


} // TEST_SUITE("Info")

} // namespace Testing
} // namespace Internals
} // namespace CoSimIO


/*
#include "co_sim_io/impl/co_sim_io_internals.h"

namespace Kratos {
namespace Testing {

typedef double double;
typedef CoSimIO::Internals::DataContainer<double> DataContainerBase;
typedef Kratos::unique_ptr<DataContainerBase> DataContainerBasePointer;
typedef CoSimIO::Internals::DataContainerStdVector<double> DataContainerStdVectorType;
typedef CoSimIO::Internals::DataContainerRawMemory<double> DataContainerRawMemoryType;

namespace {

void TestDataContainerBasics(const std::vector<double>& rRefValues, DataContainerBase& rDataContainer)
{
    // checking size
    KRATOS_CHECK_EQUAL(rRefValues.size(), rDataContainer.size());

    // checking values
    KRATOS_CHECK_VECTOR_NEAR(rRefValues, rDataContainer, 1e-12)
}

void TestDataContainerDifferentValues(const std::vector<std::vector<double>>& rRefValues, DataContainerBase& rDataContainer)
{
    for (const auto& r_current_ref_vals : rRefValues) {
        const std::size_t current_size(r_current_ref_vals.size());
        if (rDataContainer.size() != current_size) {
            rDataContainer.resize(current_size);
        }

        for (std::size_t i=0; i<current_size; ++i) {
            rDataContainer[i] = r_current_ref_vals[i];
        }

        // checking size
        KRATOS_CHECK_EQUAL(current_size, rDataContainer.size());

        // checking values
        KRATOS_CHECK_VECTOR_NEAR(r_current_ref_vals, rDataContainer, 1e-12)
    }
}

} // helpers namespace

KRATOS_TEST_CASE_IN_SUITE(DataContainers_RawMemory_empty, KratosCoSimulationFastSuite)
{
    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0] = NULL;

    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerRawMemoryType>(values_raw, 0));

    KRATOS_CHECK_EQUAL(0, p_container->size());

    // deallocating memory
    free(*values_raw);
    free(values_raw);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_StdVector_basics, KratosCoSimulationFastSuite)
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    std::vector<double> values_vec(ref_values);

    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerStdVectorType>(values_vec));

    TestDataContainerBasics(ref_values, *p_container);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_RawMemory_basics, KratosCoSimulationFastSuite)
{
    const std::vector<double> ref_values {
        1.0, -2.333, 15.88, 14.7, -99.6
    };

    const std::size_t size(ref_values.size());

    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0]= (double*)malloc(sizeof(double)*size);

    for (std::size_t i=0; i<size; ++i) {
        (*values_raw)[i] = ref_values[i];
    }

    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerRawMemoryType>(values_raw, size));

    TestDataContainerBasics(ref_values, *p_container);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_StdVector_multiple_resizes, KratosCoSimulationFastSuite)
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5},
        {-88.66, 77.9}
    };

    std::vector<double> values_vec;

    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerStdVectorType>(values_vec));

    TestDataContainerDifferentValues(ref_values, *p_container);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_RawMemory_resize_larger, KratosCoSimulationFastSuite)
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5}
    };

    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0] = NULL;
    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerRawMemoryType>(values_raw, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(*values_raw);
    free(values_raw);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_RawMemory_resize_smaller, KratosCoSimulationFastSuite)
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9}
    };

    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0] = NULL;
    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerRawMemoryType>(values_raw, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(*values_raw);
    free(values_raw);
}

KRATOS_TEST_CASE_IN_SUITE(DataContainers_RawMemory_multiple_resizes, KratosCoSimulationFastSuite)
{
    const std::vector<std::vector<double>> ref_values {
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9},
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.5},
        {1.0, -2.333, 15.88, 14.7, -99.6},
        {-88.66, 77.9}
    };

    double** values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0] = NULL;
    DataContainerBasePointer p_container(Kratos::make_unique<DataContainerRawMemoryType>(values_raw, 0));

    TestDataContainerDifferentValues(ref_values, *p_container);

    // deallocating memory
    free(*values_raw);
    free(values_raw);
}

} // namespace Testing
}  // namespace Kratos.

*/