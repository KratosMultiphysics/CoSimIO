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

#ifndef CO_SIM_IO_TESTING_INCLUDED
#define CO_SIM_IO_TESTING_INCLUDED

// External includes
#include "external_libraries/doctest/doctest.h"

// Project includes
#include "includes/model_part.hpp"

// Custom macros

#define CO_SIM_IO_CHECK_VECTOR_NEAR(a, b) {           \
REQUIRE_EQ((a).size(), (b).size());                   \
for (std::size_t _i=0; _i<(a).size(); ++_i) {         \
   CHECK_MESSAGE((a)[_i] == doctest::Approx((b)[_i]), \
   "Mismatch in component: " << _i);                  \
}                                                     \
}

#define CO_SIM_IO_CHECK_VECTOR_EQUAL(a, b) {          \
REQUIRE_EQ((a).size(), (b).size());                   \
for (std::size_t _i=0; _i<(a).size(); ++_i) {         \
   CHECK_MESSAGE((a)[_i] == (b)[_i],                  \
   "Mismatch in component: " << _i);                  \
}                                                     \
}

#define CO_SIM_IO_CHECK_VECTOR_NEAR_SIZE(a, b, s) {   \
REQUIRE_GE((a).size(), (s));                          \
REQUIRE_GE((b).size(), (s));                          \
for (std::size_t _i=0; _i<(s); ++_i) {                \
   CHECK_MESSAGE((a)[_i] == doctest::Approx((b)[_i]), \
   "Mismatch in component: " << _i);                  \
}                                                     \
}

#define CO_SIM_IO_CHECK_VECTOR_EQUAL_SIZE(a, b, s) { \
REQUIRE_GE((a).size(), (s));                         \
REQUIRE_GE((b).size(), (s));                         \
for (std::size_t _i=0; _i<(s); ++_i) {               \
   CHECK_MESSAGE((a)[_i] == (b)[_i],                 \
   "Mismatch in component: " << _i);                 \
}                                                    \
}

void CheckNodesAreEqual(
    const CoSimIO::Node& Node1,
    const CoSimIO::Node& Node2);

void CheckElementsAreEqual(
    const CoSimIO::Element& Element1,
    const CoSimIO::Element& Element2);

void CheckModelPartsAreEqual(
    const CoSimIO::ModelPart& ModelPart1,
    const CoSimIO::ModelPart& ModelPart2);

#endif // CO_SIM_IO_UTILITIES_INCLUDED
