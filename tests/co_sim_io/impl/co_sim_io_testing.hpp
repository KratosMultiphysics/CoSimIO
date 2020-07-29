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

// External includes
#include "doctest/doctest.h"

// Custom macros

#define CO_SIM_IO_CHECK_VECTOR_NEAR(a, b) { \
CHECK_EQ(a.size(), b.size());               \
for (std::size_t i=0; i<a.size(); ++i) {    \
   CHECK_EQ(a[i], doctest::Approx(b[i]));   \
}                                           \
}
