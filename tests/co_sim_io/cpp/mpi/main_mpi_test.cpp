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

#define DOCTEST_CONFIG_IMPLEMENT

#include "external_libraries/doctest/extensions/doctest_mpi.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  doctest::Context ctx;
  ctx.setOption("reporters", "MpiConsoleReporter");
  // ctx.setOption("force-colors", true); // this messes up the test detection for CTest
  ctx.applyCommandLine(argc, argv);

  int test_result = ctx.run();

  MPI_Finalize();

  return test_result;
}
