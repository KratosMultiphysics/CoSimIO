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
#include <string.h>

#include "../checks.h"

int main(int argc, const char** argv)
{
    COSIMIO_UNUSED(argc);
    COSIMIO_UNUSED(argv);

    /* declaring variables */
    CoSimIO_Info info;
    char buffer[BUFSIZ];

    info = CoSimIO_CreateInfo();

    CoSimIO_Info_SetInt(info, "echo_level", 6);

    setbuf(stdout, buffer);

    /* CoSimIO_PrintInfo(stdout, info);

    int comparison = strcmp(buffer, "CoSimIO-Info; containing 1 entries\n  name: echo_level | value: 6 | type: int\n\n");
    COSIMIO_CHECK_INT_EQUAL(comparison,0); */

    CoSimIO_FreeInfo(info);

    return 0;
}
