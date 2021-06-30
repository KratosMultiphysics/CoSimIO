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
#include "impl/serializer.h"
#include "impl/file_serializer.h"
#include "impl/stream_serializer.h"


namespace CoSimIO {

TEST_SUITE("Serializer") {


TEST_CASE("dummy")
{
    // const std::vector<double> ref_values {
    //     1.0, -2.333, 15.88, 14.7, -99.6
    // };

    // std::vector<double> values_vec(ref_values);

    // DataContainerBasePointer p_container(CoSimIO::make_unique<DataContainerStdVectorType>(values_vec));

    // CO_SIM_IO_CHECK_VECTOR_NEAR(ref_values, (*p_container));
}


} // TEST_SUITE("Serializer")

} // namespace CoSimIO
