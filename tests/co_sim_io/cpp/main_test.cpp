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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "external_libraries/doctest/doctest.h"

// Project includes
#include "includes/exception.hpp"

// registering custom exception translator
// this is required as we only want to test the error message
// and not the location which is included in the CoSimIO::Exception
REGISTER_EXCEPTION_TRANSLATOR(CoSimIO::Exception& ex) {
    return doctest::toString(ex.message());
}
