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

// Project includes
#include "../impl/code_location.h"
#include "../impl/filesystem_inc.hpp"

namespace CoSimIO {

std::string CodeLocation::GetCleanFileName() const
{
    return fs::canonical(fs::path(GetFileName())).lexically_relative(fs::absolute(".")).string();
}

}  // namespace CoSimIO.

