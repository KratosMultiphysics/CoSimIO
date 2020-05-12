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
#include <stdio.h>

// Project includes
#include "c/co_sim_io_c.h"

// #define ADD_TEST(test)
#define COSIMIO_CHECK_INT_EQUAL(a, b)                                \
    if (a != b) {                                                    \
        printf("in line %d: %d is not equal to %d", __LINE__, a, b); \
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


int info_basics_int()
{
    CoSimIO_Info info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_FALSE(CoSimIO_Info_Has(info, "echo_level"));

    CoSimIO_Info_SetInt(info, "echo_level", 6);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "echo_level"));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(info, "echo_level"), 6);

    CoSimIO_FreeInfo(info);

    return 0;
}




int main()
{

    return info_basics_int();
    // create an info object

    // do some checks with it ...

    return 0;
}
