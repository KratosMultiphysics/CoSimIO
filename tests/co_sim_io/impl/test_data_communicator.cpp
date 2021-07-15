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

TEST_CASE("DataCommunicator_serial_distributed_tests")
{
    DataCommunicator serial_communicator;

    RunAllDataCommunicatorTests(serial_communicator);
}

} // TEST_SUITE("DataCommunicator")

} // namespace Internals
} // namespace CoSimIO
