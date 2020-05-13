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
//

// CoSimulation includes
#include "c/co_sim_io_c.h"

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        printf("in line %d : %d is not equalt to %d\n", __LINE__ , a, b); \
        return 1;                                                \
    }

int main()
{
    CoSimIO_Info info = CoSimIO_Hello();

    CoSimIO_PrintInfo(stdout, info);

    int major_version = CoSimIO_Info_GetInt(info, "major_version");
    int minor_version = CoSimIO_Info_GetInt(info, "minor_version");
    const char* patch_version = CoSimIO_Info_GetString(info, "patch_version");

    COSIMIO_CHECK_EQUAL(major_version, 1);
    COSIMIO_CHECK_EQUAL(minor_version, 0);

    return 0;
}
