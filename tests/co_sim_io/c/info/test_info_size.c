/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Philipp Bucher (https://github.com/philbucher)
*/

/* Project includes */
#include "c/co_sim_io_c.h"

#include "../checks.h"

int main(int argc, const char** argv)
{
    /* declaring variables */
    CoSimIO_Info info;

    COSIMIO_UNUSED(argc);
    COSIMIO_UNUSED(argv);

    info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 0);

    CoSimIO_Info_SetInt(info, "echo_level", 6);
    CoSimIO_Info_SetString(info, "iter", "15");
    CoSimIO_Info_SetInt(info, "error_code", 0);
    CoSimIO_Info_SetBool(info, "is_converged", 1);
    CoSimIO_Info_SetDouble(info, "tolerance", 0.00331);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 5);

    CoSimIO_FreeInfo(info);

    return 0;
}
