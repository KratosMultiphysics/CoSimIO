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
#include <map>

// Project includes
#include "../impl/vtk_utilities.hpp"
#include "../impl/macros.hpp"

namespace CoSimIO {
namespace Internals {

VtkCellType GetVtkCellTypeForElementType(ElementType I_ElementType)
{
    const std::map<ElementType, VtkCellType> element_type_to_cell_Type_map = {
        {ElementType::Hexahedra3D20,        VtkCellType::Quadratic_Hexahedron},
        {ElementType::Hexahedra3D8,         VtkCellType::Hexahedron},
        {ElementType::Prism3D6,             VtkCellType::Wedge},
        {ElementType::Quadrilateral2D4,     VtkCellType::Quad},
        {ElementType::Quadrilateral2D8,     VtkCellType::Quadratic_Quad},
        {ElementType::Quadrilateral3D4,     VtkCellType::Quad},
        {ElementType::Quadrilateral3D8,     VtkCellType::Quadratic_Quad},
        {ElementType::Tetrahedra3D10,       VtkCellType::Quadratic_Tetra},
        {ElementType::Tetrahedra3D4,        VtkCellType::Tetra},
        {ElementType::Triangle2D3,          VtkCellType::Triangle},
        {ElementType::Triangle2D6,          VtkCellType::Quadratic_Triangle},
        {ElementType::Triangle3D3,          VtkCellType::Triangle},
        {ElementType::Triangle3D6,          VtkCellType::Quadratic_Triangle},
        {ElementType::Line2D2,              VtkCellType::Line},
        {ElementType::Line2D3,              VtkCellType::Quadratic_Edge},
        {ElementType::Line3D2,              VtkCellType::Line},
        {ElementType::Line3D3,              VtkCellType::Quadratic_Edge},
        {ElementType::Point2D,              VtkCellType::Vertex},
        {ElementType::Point3D,              VtkCellType::Vertex}
    };

    auto type_iter = element_type_to_cell_Type_map.find(I_ElementType);
    CO_SIM_IO_ERROR_IF(type_iter == element_type_to_cell_Type_map.end()) << "Unsupported element type: " << static_cast<int>(I_ElementType) << std::endl; // TODO maybe return -1 or so here in the future. This way also types not supported by vtk/Paraview could be used with CoSomIO (but not visualized in Paraview)
    return type_iter->second;
}

} // namespace Internals
} // namespace CoSimIO
