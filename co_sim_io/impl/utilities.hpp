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
#include <map>

// Project includes
#include "define.hpp"

namespace CoSimIO {
namespace Internals {

// Create the name for the connection
// In a function bcs maybe in the future this will
// need to be more elaborate
inline std::string CreateConnectionName(
    const std::string& rName1,
    const std::string& rName2)
{
    if (rName1 < rName2) {
        return rName1 + "_" + rName2;
    } else {
        return rName2 + "_" + rName1;
    }
}

inline int GetNumberOfNodesForElementType(ElementType Type)
{
    const std::map<ElementType, int> vtk_cell_type_map {
        { ElementType::VERTEX,     1},
        { ElementType::LINE,       2},
        { ElementType::TRIANGLE,   3},
        { ElementType::QUAD,       4},
        { ElementType::TETRA,      4},
        { ElementType::HEXAHEDRON, 8}
    };

    auto type_iter = vtk_cell_type_map.find(Type);
    CO_SIM_IO_ERROR_IF(type_iter == vtk_cell_type_map.end()) << "Unsupported cell type: " << Type << std::endl;
    return type_iter->second;
}

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_UTILITIES_INCLUDED
