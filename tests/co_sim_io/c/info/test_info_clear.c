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

    info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 0);

    CoSimIO_Info_SetInt(info, "echo_level", 6);
    CoSimIO_Info_SetString(info, "iter", "15");
    CoSimIO_Info_SetBool(info, "error_code", 0);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "echo_level"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "iter"));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 3);

    CoSimIO_Info_Clear(info);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 0);

    CoSimIO_FreeInfo(info);

    return 0;
}
