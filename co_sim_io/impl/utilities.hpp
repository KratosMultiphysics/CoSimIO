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
        {ElementType::Hexahedra3D20,        20},
        {ElementType::Hexahedra3D27,        27},
        {ElementType::Hexahedra3D8,         8},
        {ElementType::Prism3D15,            15},
        {ElementType::Prism3D6,             6},
        {ElementType::Quadrilateral2D4,     4},
        {ElementType::Quadrilateral2D8,     8},
        {ElementType::Quadrilateral2D9,     9},
        {ElementType::Quadrilateral3D4,     4},
        {ElementType::Quadrilateral3D8,     8},
        {ElementType::Quadrilateral3D9,     9},
        {ElementType::Tetrahedra3D10,       10},
        {ElementType::Tetrahedra3D4,        4},
        {ElementType::Triangle2D3,          3},
        {ElementType::Triangle2D6,          6},
        {ElementType::Triangle3D3,          3},
        {ElementType::Triangle3D6,          6},
        {ElementType::Line2D2,              2},
        {ElementType::Line2D3,              3},
        {ElementType::Line3D2,              2},
        {ElementType::Line3D3,              3},
        {ElementType::Point2D,              1},
        {ElementType::Point3D,              1}
    };

    auto type_iter = vtk_cell_type_map.find(Type);
    CO_SIM_IO_ERROR_IF(type_iter == vtk_cell_type_map.end()) << "Unsupported cell type: " << Type << std::endl;
    return type_iter->second;
}

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_UTILITIES_INCLUDED
