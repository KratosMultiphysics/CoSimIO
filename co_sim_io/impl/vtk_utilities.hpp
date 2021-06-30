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

// Project includes
#include "define.hpp"

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

VtkCellType GetVtkCellTypeForElementType(ElementType I_ElementType);

} // namespace Internals
} // namespace CoSimIO

#endif // CO_SIM_IO_VTK_UTILITIES_INCLUDED
