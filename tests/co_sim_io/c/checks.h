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

// this file contains some test-macros

// System includes
#include <stdio.h>
#include <math.h>

#define COSIMIO_CHECK_INT_EQUAL(a, b)                                \
    if (a != b) {                                                    \
        printf("in line %d: %d is not equal to %d", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_DOUBLE_EQUAL(a, b)                             \
    if (fabs(a-b)>1e-15) {                                           \
        printf("in line %d: %f is not equal to %f", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_TRUE(a)                                        \
    if (a!=1) {                                                      \
        printf("in line %d: %d is not true!", __LINE__, a);          \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_FALSE(a)                                       \
    if (a!=0) {                                                      \
        printf("in line %d: %d is not false!", __LINE__, a);         \
        return 1;                                                    \
    }
