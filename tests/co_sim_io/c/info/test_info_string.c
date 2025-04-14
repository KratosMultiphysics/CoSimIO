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
    COSIMIO_UNUSED(argc);
    COSIMIO_UNUSED(argv);

    /* declaring variables */
    CoSimIO_Info info;

    info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_FALSE(CoSimIO_Info_Has(info, "identifier"));

    CoSimIO_Info_SetString(info, "identifier", "field_pressure_interface");

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "identifier"));

    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_Info_GetString(info, "identifier"), "field_pressure_interface");

    CoSimIO_FreeInfo(info);

    return 0;
}
