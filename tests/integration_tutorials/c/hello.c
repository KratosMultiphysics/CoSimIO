//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Pooyan Dadvand


//// System includes
#include <stdio.h>
#include <string.h>

// CoSimulation includes
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

#define COSIMIO_CHECK_STRING_EQUAL(a, b)                             \
    if (strcmp(a,b)) {                                               \
        printf("in line %d: %s is not equal to %s", __LINE__, a, b); \
        return 1;                                                    \
    }

int main()
{
    CoSimIO_Info hello_info = CoSimIO_Hello();

    CoSimIO_PrintInfo(stdout, hello_info);

    int major_version = CoSimIO_Info_GetInt(hello_info, "major_version");
    int minor_version = CoSimIO_Info_GetInt(hello_info, "minor_version");
    const char* patch_version = CoSimIO_Info_GetString(hello_info, "patch_version");

    // COSIMIO_CHECK_EQUAL(major_version, 1);
    // COSIMIO_CHECK_EQUAL(minor_version, 0);
    // COSIMIO_CHECK_STRING_EQUAL(patch_version, "0");

    CoSimIO_FreeInfo(hello_info);

    return 0;
}
