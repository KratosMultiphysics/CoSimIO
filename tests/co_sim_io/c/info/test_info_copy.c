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

int main()
{
    /* declaring variables */
    CoSimIO_Info info, copied_info;

    info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 0);

    CoSimIO_Info_SetInt(info, "echo_level", 6);
    CoSimIO_Info_SetString(info, "iter", "15");
    CoSimIO_Info_SetInt(info, "error_code", 0);
    CoSimIO_Info_SetBool(info, "is_converged", 1);
    CoSimIO_Info_SetDouble(info, "tolerance", 0.00331);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "echo_level"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "iter"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "error_code"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "is_converged"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "tolerance"));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(info, "echo_level"), 6);
    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_Info_GetString(info, "iter"), "15");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(info, "error_code"), 0);
    COSIMIO_CHECK_TRUE(CoSimIO_Info_GetBool(info, "is_converged"));
    COSIMIO_CHECK_DOUBLE_EQUAL(CoSimIO_Info_GetDouble(info, "tolerance"), 0.00331);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 5);

    copied_info = CoSimIO_CopyInfo(info);
    CoSimIO_FreeInfo(info); /* delete this here to make sure "copied_info" is a deep and not a shallow copy */

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(copied_info, "echo_level"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(copied_info, "iter"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(copied_info, "error_code"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(copied_info, "is_converged"));
    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(copied_info, "tolerance"));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(copied_info, "echo_level"), 6);
    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_Info_GetString(copied_info, "iter"), "15");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(copied_info, "error_code"), 0);
    COSIMIO_CHECK_TRUE(CoSimIO_Info_GetBool(copied_info, "is_converged"));
    COSIMIO_CHECK_DOUBLE_EQUAL(CoSimIO_Info_GetDouble(copied_info, "tolerance"), 0.00331);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(copied_info), 5);

    CoSimIO_FreeInfo(copied_info);

    return 0;
}
