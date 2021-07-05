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

#ifndef CO_SIM_IO_UTILITIES_INCLUDED
#define CO_SIM_IO_UTILITIES_INCLUDED

// System includes
#include <string>
#include <chrono>

// Project includes
#include "define.hpp"

namespace CoSimIO {
namespace Internals {

// Create the name for the connection
// In a function bcs maybe in the future this will
// need to be more elaborate
std::string CO_SIM_IO_API CreateConnectionName(
    const std::string& rName1,
    const std::string& rName2);

void CO_SIM_IO_API CheckEntry(const std::string& rEntry, const std::string& rKey);

double CO_SIM_IO_API ElapsedSeconds(const std::chrono::steady_clock::time_point& rStartTime);

int CO_SIM_IO_API GetNumberOfNodesForElementType(ElementType Type);

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_UTILITIES_INCLUDED
