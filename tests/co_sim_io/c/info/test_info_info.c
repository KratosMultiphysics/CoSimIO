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
    CoSimIO_Info sub_info;
    CoSimIO_Info retrieved_sub_info;

    info = CoSimIO_CreateInfo();
    sub_info = CoSimIO_CreateInfo();

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(info), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(sub_info), 0);

    CoSimIO_Info_SetInt(sub_info, "echo_level", 6);
    CoSimIO_Info_SetString(sub_info, "iter", "15");
    CoSimIO_Info_SetInt(sub_info, "error_code", 0);
    CoSimIO_Info_SetBool(sub_info, "is_converged", 1);
    CoSimIO_Info_SetDouble(sub_info, "tolerance", 0.00331);

    CoSimIO_Info_SetInfo(info, "sub_settings", sub_info);

    COSIMIO_CHECK_TRUE(CoSimIO_Info_Has(info, "sub_settings"));

    retrieved_sub_info = CoSimIO_Info_GetInfo(info, "sub_settings");

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(retrieved_sub_info, "echo_level"), 6);
    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_Info_GetString(retrieved_sub_info, "iter"), "15");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_GetInt(retrieved_sub_info, "error_code"), 0);
    COSIMIO_CHECK_TRUE(CoSimIO_Info_GetBool(retrieved_sub_info, "is_converged"));
    COSIMIO_CHECK_DOUBLE_EQUAL(CoSimIO_Info_GetDouble(retrieved_sub_info, "tolerance"), 0.00331);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Info_Size(retrieved_sub_info), 5);

    CoSimIO_FreeInfo(info);
    CoSimIO_FreeInfo(sub_info);
    CoSimIO_FreeInfo(retrieved_sub_info);

    return 0;
}
