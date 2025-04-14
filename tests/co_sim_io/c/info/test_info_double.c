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

    COSIMIO_CHECK_FALSE(CoSimIO_Info_Has(info, "tolerance"));

    CoSimIO_Info_SetDouble(info, "tolerance", 0.0086);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "tolerance"));

    COSIMIO_CHECK_DOUBLE_EQUAL(CoSimIO_Info_GetDouble(info, "tolerance"), 0.0086);

    CoSimIO_FreeInfo(info);

    return 0;
}
