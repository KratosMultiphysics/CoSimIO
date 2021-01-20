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

#ifndef CO_SIM_IO_VTK_UTILITIES_INCLUDED
#define CO_SIM_IO_VTK_UTILITIES_INCLUDED

// System includes
#include <map>

// Project includes
#include "define.hpp"
#include "macros.hpp"

namespace CoSimIO {
namespace Internals {

// see https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf ; figure 2
enum class VtkCellType {
    Vertex = 1,
    Line = 3,
    Triangle = 5,
    Quad = 9,
    Tetra = 10,
    Hexahedron = 12,
    Wedge = 13,
    Pyramid = 14,

    Quadratic_Edge = 21,
    Quadratic_Triangle = 22,
    Quadratic_Quad = 23,
    Quadratic_Tetra = 24,
    Quadratic_Hexahedron = 25,
};

// TODO make constexpr, static etc?
inline std::map<VtkCellType, ElementType> GetVtkCellTypeToElementTypeMap()
{
    return {
        {VtkCellType::Quadratic_Hexahedron,     ElementType::Hexahedra3D20},
        {VtkCellType::Hexahedron,               ElementType::Hexahedra3D8},
        {VtkCellType::Wedge,                    ElementType::Prism3D6},
        {VtkCellType::Quad,                     ElementType::Quadrilateral2D4},
        {VtkCellType::Quadratic_Quad,           ElementType::Quadrilateral2D8},
        {VtkCellType::Quad,                     ElementType::Quadrilateral3D4},
        {VtkCellType::Quadratic_Quad,           ElementType::Quadrilateral3D8},
        {VtkCellType::Quadratic_Tetra,          ElementType::Tetrahedra3D10},
        {VtkCellType::Tetra,                    ElementType::Tetrahedra3D4},
        {VtkCellType::Triangle,                 ElementType::Triangle2D3},
        {VtkCellType::Quadratic_Triangle,       ElementType::Triangle2D6},
        {VtkCellType::Triangle,                 ElementType::Triangle3D3},
        {VtkCellType::Quadratic_Triangle,       ElementType::Triangle3D6},
        {VtkCellType::Line,                     ElementType::Line2D2},
        {VtkCellType::Quadratic_Edge,           ElementType::Line2D3},
        {VtkCellType::Line,                     ElementType::Line3D2},
        {VtkCellType::Quadratic_Edge,           ElementType::Line3D3},
        {VtkCellType::Vertex,                   ElementType::Point2D},
        {VtkCellType::Vertex,                   ElementType::Point3D}
    };
}

inline ElementType GetElementTypeForVtkCellType(VtkCellType I_CellType)
{
    const auto cell_type_element_type_map = GetVtkCellTypeToElementTypeMap();

    auto type_iter = cell_type_element_type_map.find(I_CellType);
    CO_SIM_IO_ERROR_IF(type_iter == cell_type_element_type_map.end()) << "Unsupported vtk cell type: " << static_cast<int>(I_CellType) << std::endl;
    return type_iter->second;
}

inline VtkCellType GetVtkCellTypeForElementType(ElementType I_ElementType)
{
    const auto cell_type_element_type_map = GetVtkCellTypeToElementTypeMap();

    for (auto it=cell_type_element_type_map.begin(); it!=cell_type_element_type_map.end(); ++it) {
        if (it->second == I_ElementType) {
            return it->first;
        }
    }

    CO_SIM_IO_ERROR << "Unsupported element type: " << static_cast<int>(I_ElementType) << std::endl;
}

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_VTK_UTILITIES_INCLUDED
