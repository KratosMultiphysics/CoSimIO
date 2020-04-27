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


// System includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test includes
#include "interface/interface_tests.h"
#include "solver_integration/connect_disconnect/solver.h"
#include "solver_integration/data_exchange/solver.h"
#include "solver_integration/mesh_exchange/solver.h"

// #define CONNECTION_NAME "tester_C"

// // #define CHECK_TRUE() ...
// // #define CHECK_FALSE() ...
// #define CHECK_EQAUL(a, b) ...


// int Test_CoSimIO_Info()
// {
//     int test_exit_code = 0;
//     CoSimIO_Info c_info;

//     const int val = CoSimIO_GetInt(c_info, "some_val_non_existant");

//     CHECK_EQAUL(val, 0)

//     return test_exit_code;
// }

// int Test_IsConnected()
// {
//     int test_exit_code = 0;

//     CoSimIO_ConnectionSettings settings;

//     CoSimIO_ReturnInfo c_info = CoSimIO_Connect(CONNECTION_NAME, settings);

//     CHECK_EQAUL(test_exit_code, c_info.GetReturnCode(), 0);


//     CoSimIO_ReturnInfo dc_info = CoSimIO_Disconnect(CONNECTION_NAME);
//     CHECK_EQAUL(test_exit_code, dc_info.GetReturnCode(), 0);

//     return test_exit_code;
// }



// void RunTestCase(const int TestId)
// {
//     switch (TestId)
//     ​{
//     case 1:
//       exit(Test_IsConnected());
//       break;

//     case 2:
//       // statements
//       break;
//     default:
//       // default statements
//     }





// }

int main(int argc, char **argv)
{
    // parsing input
    if (argc != 2) {
        printf("One input argument required: Name of the test to execute");
        exit(1);
    }

    const char* test_name = argv[1];

    int return_code = 0;

    if (strcmp(test_name, "test_Info") == 0) {
        // return_code = CallMyTest();
    } else if (strcmp(test_name, "test_Iddddnfo") == 0) {

    } else {
        printf("Test name is unknown: %s", test_name);
        return_code = 1;
    }

    return return_code;
}