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

// Project includes
#include "c/co_sim_io_c.h"

#include "../checks.h"

int main()
{
    CoSimIO_Info info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_FALSE(CoSimIO_Info_Has(info, "is_converged"));

    CoSimIO_Info_SetBool(info, "is_converged", 1);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "is_converged"));

    COSIMIO_CHECK_TRUE(CoSimIO_Info_GetBool(info, "is_converged"));

    CoSimIO_FreeInfo(info);

    return 0;
}
