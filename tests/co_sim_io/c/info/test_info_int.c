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

int main(int COSIMIO_MAYBE_UNUSED(argc), const char** COSIMIO_MAYBE_UNUSED(argv))
{
    /* declaring variables */
    CoSimIO_Info info;

    info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_FALSE(CoSimIO_Info_Has(info, "echo_level"));

    CoSimIO_Info_SetInt(info, "echo_level", 6);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "echo_level"));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(info, "echo_level"), 6);

    CoSimIO_FreeInfo(info);

    return 0;
}
