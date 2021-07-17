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


/*
KRATOS_TEST_CASE_IN_SUITE(DataCommuniactorScanSumInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    int local = 1;
    int result = serial_communicator.ScanSum(local);
    KRATOS_CHECK_EQUAL(result, local);
}

KRATOS_TEST_CASE_IN_SUITE(DataCommuniactorScanSumDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    double local = 2.0;
    double result = serial_communicator.ScanSum(local);
    KRATOS_CHECK_EQUAL(result, local);
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScanSumIntVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    std::vector<int> local{1, 1};
    std::vector<int> output{-1, -1};

    // two-buffer version
    serial_communicator.ScanSum(local, output);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(output[i], local[i]);
    }

    // return buffer version
    std::vector<int> returned_result = serial_communicator.ScanSum(local);
    KRATOS_CHECK_EQUAL(returned_result.size(), 2);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(returned_result[i], local[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_global{-1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.ScanSum(local, wrong_size_global),
        "Input error in call to DataCommunicator::ScanSum"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScanSumDoubleVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    std::vector<double> local{2.0, 2.0};
    std::vector<double> output{-1.0, -1.0};

    // two-buffer version
    serial_communicator.ScanSum(local, output);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(output[i], local[i]);
    }

    // return buffer version
    std::vector<double> returned_result = serial_communicator.ScanSum(local);
    KRATOS_CHECK_EQUAL(returned_result.size(), 2);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(returned_result[i], local[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_global{-1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.ScanSum(local, wrong_size_global),
        "Input error in call to DataCommunicator::ScanSum"
    );
    #endif
}

// SendRecv ///////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorSendRecvInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();

    const int world_size = r_world.Size();
    const int world_rank = r_world.Rank();

    // Serial communication is only well defined for the trivial case.
    int send_rank = 0;
    int recv_rank = 0;

    std::vector<int> send_buffer = {world_rank, world_rank};
    std::vector<int> recv_buffer = {-1, -1};

    // two-buffer version
    serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<int> return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.SendRecv(send_buffer, send_rank, 0, wrong_size_recv, recv_rank, 0),
        "Input error in call to DataCommunicator::SendRecv"
    );
    #endif

    // remote calls are not supported
    if (world_size > 2) {
        send_rank = world_rank + 1 == world_size ? 0 : world_rank + 1;
        recv_rank = world_rank == 0 ? world_size - 1 : world_rank - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorSendRecvDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();

    const int world_size = r_world.Size();
    const int world_rank = r_world.Rank();

    // Serial communication is only well defined for the trivial case.
    int send_rank = 0;
    int recv_rank = 0;

    std::vector<double> send_buffer = {2.0*world_rank, 2.0*world_rank};
    std::vector<double> recv_buffer = {-1.0, -1.0};

    // two-buffer version
    serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<double> return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.SendRecv(send_buffer, send_rank, 0, wrong_size_recv, recv_rank, 0),
        "Input error in call to DataCommunicator::SendRecv"
    );
    #endif

    // remote calls are not supported
    if (world_size > 2) {
        send_rank = world_rank + 1 == world_size ? 0 : world_rank + 1;
        recv_rank = world_rank == 0 ? world_size - 1 : world_rank - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorSendRecvString, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();

    const int world_size = r_world.Size();
    const int world_rank = r_world.Rank();

    // Serial communication is only well defined for the trivial case.
    int send_rank = 0;
    int recv_rank = 0;

    std::string send_buffer("Hello world!");
    // The output is only needed to be of the same size, I initialize it to check it later.
    std::string recv_buffer("************");

    // two-buffer version
    serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0);
    KRATOS_CHECK_C_STRING_EQUAL(recv_buffer.c_str(), "Hello world!");

    // return version
    std::string return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank);
    KRATOS_CHECK_C_STRING_EQUAL(recv_buffer.c_str(), "Hello world!");

    #ifdef KRATOS_DEBUG
    std::string wrong_size_recv("*");
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.SendRecv(send_buffer, send_rank, 0, wrong_size_recv, recv_rank, 0),
        "Input error in call to DataCommunicator::SendRecv"
    );
    #endif

    // remote calls are not supported
    if (world_size > 2) {
        send_rank = world_rank + 1 == world_size ? 0 : world_rank + 1;
        recv_rank = world_rank == 0 ? world_size - 1 : world_rank - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.SendRecv(send_buffer, send_rank, 0, recv_buffer, recv_rank, 0),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.SendRecv(send_buffer, send_rank, recv_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }
}

// Broadcast //////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorBroadcastInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_rank = r_world.Rank();
    const int send_rank = 0;

    int send = 1 + world_rank;
    serial_communicator.Broadcast(send,send_rank);
    KRATOS_CHECK_EQUAL(send, 1 + world_rank);
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorBroadcastDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_rank = r_world.Rank();
    const int send_rank = 0;

    double send = 1.0 + world_rank;
    serial_communicator.Broadcast(send,send_rank);
    KRATOS_CHECK_EQUAL(send, 1.0 + world_rank);
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorBroadcastIntVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_rank = r_world.Rank();
    const int send_rank = 0;

    std::vector<int> send = {world_rank, 1 + world_rank};
    serial_communicator.Broadcast(send,send_rank);
    KRATOS_CHECK_EQUAL(send[0], world_rank);
    KRATOS_CHECK_EQUAL(send[1], 1 + world_rank);
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorBroadcastDoubleVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_rank = r_world.Rank();
    const int send_rank = 0;

    std::vector<double> send = {1.0*world_rank, 1.0 + world_rank};
    serial_communicator.Broadcast(send,send_rank);
    KRATOS_CHECK_EQUAL(send[0], world_rank);
    KRATOS_CHECK_EQUAL(send[1], 1 + world_rank);
}

// Scatter ////////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScatterIntVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    std::vector<int> send_buffer = {1, 1};
    std::vector<int> recv_buffer = {-1, -1};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Scatter(send_buffer, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<int> return_buffer = serial_communicator.Scatter(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Scatter(send_buffer, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Scatter(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1, -1, -1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatter(send_buffer, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Scatter"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScatterDoubleVector, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    std::vector<double> send_buffer = {2.0, 2.0};
    std::vector<double> recv_buffer = {-1.0, -1.0};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Scatter(send_buffer, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<double> return_buffer = serial_communicator.Scatter(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Scatter(send_buffer, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Scatter(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0, -1.0, -1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatter(send_buffer, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Scatter"
    );
    #endif
}

// Scatterv ///////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScattervInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatterv only works for the trivial case (from 0 to 0)
    std::vector<int> send_buffer_single = {1, 1};

    std::vector<std::vector<int>> send_buffer_multiple = {send_buffer_single};
    std::vector<int> send_offsets = {0};
    std::vector<int> send_counts = {2};

    std::vector<int> recv_buffer = {-1, -1};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer_single[i]);
    }

    // return version
    std::vector<int> return_buffer = serial_communicator.Scatterv(send_buffer_multiple, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer_single.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer_single[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Scatterv(send_buffer_multiple, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1, -1, -1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    std::vector<int> wrong_counts = {2, 3};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, wrong_counts, send_offsets, recv_buffer, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    std::vector<int> wrong_offsets = {0, 1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, send_counts, wrong_offsets, recv_buffer, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorScattervDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatterv only works for the trivial case (from 0 to 0)
    std::vector<double> send_buffer_single = {2.0, 2.0};

    std::vector<std::vector<double>> send_buffer_multiple = {send_buffer_single};
    std::vector<int> send_offsets = {0};
    std::vector<int> send_counts = {2};

    std::vector<double> recv_buffer = {-1.0, -1.0};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer_single[i]);
    }

    // return version
    std::vector<double> return_buffer = serial_communicator.Scatterv(send_buffer_multiple, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer_single.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer_single[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Scatterv(send_buffer_multiple, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0, -1.0, -1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, send_counts, send_offsets, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    std::vector<int> wrong_counts = {2, 3};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, wrong_counts, send_offsets, recv_buffer, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    std::vector<int> wrong_offsets = {0, 1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Scatterv(send_buffer_single, send_counts, wrong_offsets, recv_buffer, send_rank),
        "Input error in call to DataCommunicator::Scatterv"
    );
    #endif
}

// Gather /////////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorGatherInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    std::vector<int> send_buffer = {1, 1};
    std::vector<int> recv_buffer = {-1, -1};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Gather(send_buffer, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<int> return_buffer = serial_communicator.Gather(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Gather(send_buffer, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Gather(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1, -1, -1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gather(send_buffer, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Gather"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorGatherDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    std::vector<double> send_buffer = {2.0, 2.0};
    std::vector<double> recv_buffer = {-1.0, -1.0};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Gather(send_buffer, recv_buffer, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<double> return_buffer = serial_communicator.Gather(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Gather(send_buffer, recv_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Gather(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0, -1.0, -1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gather(send_buffer, wrong_size_recv, send_rank),
        "Input error in call to DataCommunicator::Gather"
    );
    #endif
}

// Gatherv ////////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorGathervInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatterv only works for the trivial case (from 0 to 0)
    std::vector<int> send_buffer = {1, 1};

    std::vector<int> recv_offsets = {0};
    std::vector<int> recv_counts = {2};

    std::vector<int> recv_buffer = {-1, -1};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, recv_offsets, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<std::vector<int>> return_buffer = serial_communicator.Gatherv(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), 1);
    KRATOS_CHECK_EQUAL(return_buffer[0].size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[0][i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, recv_offsets, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Gatherv(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1, -1, -1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, wrong_size_recv, recv_counts, recv_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    std::vector<int> wrong_counts = {2, 3};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, recv_buffer, wrong_counts, recv_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    std::vector<int> wrong_offsets = {0, 1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, wrong_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorGathervDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatterv only works for the trivial case (from 0 to 0)
    std::vector<double> send_buffer = {2.0, 2.0};

    std::vector<int> recv_offsets = {0};
    std::vector<int> recv_counts = {2};

    std::vector<double> recv_buffer = {-1.0, -1.0};
    int send_rank = 0;

    // two-buffer version
    serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, recv_offsets, send_rank);
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return version
    std::vector<std::vector<double>> return_buffer = serial_communicator.Gatherv(send_buffer, send_rank);
    KRATOS_CHECK_EQUAL(return_buffer.size(), 1);
    KRATOS_CHECK_EQUAL(return_buffer[0].size(), send_buffer.size());
    for (int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[0][i], send_buffer[i]);
    }

    // remote calls are not supported
    const DataCommunicator& r_world = DataCommunicator::GetDefault();
    const int world_size = r_world.Size();

    if (world_size > 1) {
        send_rank = world_size - 1;

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, recv_offsets, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );

        KRATOS_CHECK_EXCEPTION_IS_THROWN(
            return_buffer = serial_communicator.Gatherv(send_buffer, send_rank),
            "Communication between different ranks is not possible with a serial DataCommunicator."
        );
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0, -1.0, -1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, wrong_size_recv, recv_counts, recv_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    std::vector<int> wrong_counts = {2, 3};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, recv_buffer, wrong_counts, recv_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    std::vector<int> wrong_offsets = {0, 1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.Gatherv(send_buffer, recv_buffer, recv_counts, wrong_offsets, send_rank),
        "Input error in call to DataCommunicator::Gatherv"
    );
    #endif
}

// AllGather //////////////////////////////////////////////////////////////////

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorAllGatherInt, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    const std::vector<int> send_buffer = {1, 1};
    std::vector<int> recv_buffer = {-1, -1};

    // two-buffer version
    serial_communicator.AllGather(send_buffer, recv_buffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return buffer version
    std::vector<int> return_buffer = serial_communicator.AllGather(send_buffer);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (unsigned int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<int> wrong_size_recv = {-1, -1, -1};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.AllGather(send_buffer, wrong_size_recv),
        "Input error in call to DataCommunicator::AllGather"
    );
    #endif
}

KRATOS_TEST_CASE_IN_SUITE(DataCommunicatorAllGatherDouble, KratosMPICoreFastSuite)
{
    DataCommunicator serial_communicator;

    // the serial version of scatter only works for the trivial case (from 0 to 0)
    const std::vector<double> send_buffer = {2.0, 2.0};
    std::vector<double> recv_buffer = {-1.0, -1.0};

    // two-buffer version
    serial_communicator.AllGather(send_buffer, recv_buffer);
    for (unsigned int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(recv_buffer[i], send_buffer[i]);
    }

    // return buffer version
    std::vector<double> return_buffer = serial_communicator.AllGather(send_buffer);
    KRATOS_CHECK_EQUAL(return_buffer.size(), send_buffer.size());
    for (unsigned int i = 0; i < 2; i++)
    {
        KRATOS_CHECK_EQUAL(return_buffer[i], send_buffer[i]);
    }

    #ifdef KRATOS_DEBUG
    std::vector<double> wrong_size_recv = {-1.0, -1.0, -1.0};
    KRATOS_CHECK_EXCEPTION_IS_THROWN(
        serial_communicator.AllGather(send_buffer, wrong_size_recv),
        "Input error in call to DataCommunicator::AllGather"
    );
    #endif
}
*/

} // TEST_SUITE("DataCommunicator")

} // namespace Internals
} // namespace CoSimIO
