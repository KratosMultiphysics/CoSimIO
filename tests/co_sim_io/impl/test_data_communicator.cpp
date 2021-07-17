//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Jordi Cotela
//                   Philipp Bucher (https://github.com/philbucher)
//
// Ported from Kratos
//

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/data_communicator.hpp"
#include "data_communicator_tests_serial_distributed.hpp"

namespace CoSimIO {
namespace Internals { // DataCommunicator is in "Internals" namespace

TEST_SUITE("DataCommunicator")
{

TEST_CASE("DataCommunicator_RankAndSize")
{
    DataCommunicator serial_communicator;

    CHECK_EQ(serial_communicator.Rank(), 0);
    CHECK_EQ(serial_communicator.Size(), 1);
}

TEST_CASE("DataCommunicator_InquiryChecks")
{
    DataCommunicator serial_communicator;

    CHECK_UNARY(serial_communicator.IsDefinedOnThisRank());
    CHECK_UNARY_FALSE(serial_communicator.IsNullOnThisRank());
    CHECK_UNARY_FALSE(serial_communicator.IsDistributed());
}

TEST_CASE("DataCommunicator_ErrorBroadcasting")
{
    DataCommunicator serial_communicator;

    // The serial communicator does not throw,
    // since it does not know about "other ranks" to broadcast the error to.
    // All these functions need to do is to pass along the bool condition.
    CHECK_UNARY(serial_communicator.BroadcastErrorIfTrue(true, 0));
    CHECK_UNARY_FALSE(serial_communicator.BroadcastErrorIfTrue(false, 0));
    CHECK_UNARY(serial_communicator.BroadcastErrorIfFalse(true, 0));
    CHECK_UNARY_FALSE(serial_communicator.BroadcastErrorIfFalse(false, 0));

    CHECK_UNARY(serial_communicator.ErrorIfTrueOnAnyRank(true));
    CHECK_UNARY_FALSE(serial_communicator.ErrorIfTrueOnAnyRank(false));
    CHECK_UNARY(serial_communicator.ErrorIfFalseOnAnyRank(true));
    CHECK_UNARY_FALSE(serial_communicator.ErrorIfFalseOnAnyRank(false));
}

TEST_CASE("DataCommunicator_sum_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    int local = 1;
    int result = serial_communicator.Sum(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_sum_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    double local = 1.58;
    double result = serial_communicator.Sum(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_sum_vec_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.Sum(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.Sum(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Sum(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Sum: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_sum_vec_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.Sum(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.Sum(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Sum(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Sum: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_min_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    int local = 1;
    int result = serial_communicator.Min(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_min_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    double local = 1.58;
    double result = serial_communicator.Min(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_min_vec_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.Min(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.Min(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Min(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Min: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_min_vec_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.Min(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.Min(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Min(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Min: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_max_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    int local = 1;
    int result = serial_communicator.Max(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_max_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    double local = 1.58;
    double result = serial_communicator.Max(local, root);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_max_vec_int")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.Max(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.Max(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Max(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Max: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_max_vec_double")
{
    DataCommunicator serial_communicator;
    constexpr int root = 0;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.Max(local, output, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.Max(local, root);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.Max(local, wrong_size_global, root),
        "Error: Input error in call to DataCommunicator::Max: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_sum_all_int")
{
    DataCommunicator serial_communicator;
    int local = 1;
    int result = serial_communicator.SumAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_sum_all_double")
{
    DataCommunicator serial_communicator;

    double local = 1.58;
    double result = serial_communicator.SumAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_sum_all_vec_int")
{
    DataCommunicator serial_communicator;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.SumAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.SumAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.SumAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::SumAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_sum_all_vec_double")
{
    DataCommunicator serial_communicator;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.SumAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.SumAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.SumAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::SumAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_min_all_int")
{
    DataCommunicator serial_communicator;

    int local = 1;
    int result = serial_communicator.MinAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_min_all_double")
{
    DataCommunicator serial_communicator;

    double local = 1.58;
    double result = serial_communicator.MinAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_min_all_vec_int")
{
    DataCommunicator serial_communicator;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.MinAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.MinAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.MinAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::MinAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_min_all_vec_double")
{
    DataCommunicator serial_communicator;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.MinAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.MinAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.MinAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::MinAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_max_all_int")
{
    DataCommunicator serial_communicator;

    int local = 1;
    int result = serial_communicator.MaxAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_max_all_double")
{
    DataCommunicator serial_communicator;

    double local = 1.58;
    double result = serial_communicator.MaxAll(local);
    CHECK_EQ(result, local);
}

TEST_CASE("DataCommunicator_max_all_vec_int")
{
    DataCommunicator serial_communicator;

    std::vector<int> local{1, 1};
    std::vector<int> output(local.size());

    // two-buffer version
    serial_communicator.MaxAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<int> returned_result = serial_communicator.MaxAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<int> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.MaxAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::MaxAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_max_all_vec_double")
{
    DataCommunicator serial_communicator;

    std::vector<double> local{1.2, 1.87};
    std::vector<double> output(local.size());

    // two-buffer version
    serial_communicator.MaxAll(local, output);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(output, local)

    // return buffer version
    std::vector<double> returned_result = serial_communicator.MaxAll(local);
    CO_SIM_IO_CHECK_VECTOR_EQUAL(returned_result, local)

    #ifdef CO_SIM_IO_DEBUG
    std::vector<double> wrong_size_global{-1};
    CHECK_THROWS_WITH(
        serial_communicator.MaxAll(local, wrong_size_global),
        "Error: Input error in call to DataCommunicator::MaxAll: The sizes of the local and distributed buffers do not match.\n"
    );
    #endif
}

TEST_CASE("DataCommunicator_serial_distributed_tests")
{
    DataCommunicator serial_communicator;

    RunAllDataCommunicatorTests(serial_communicator);
}

} // TEST_SUITE("DataCommunicator")

} // namespace Internals
} // namespace CoSimIO
