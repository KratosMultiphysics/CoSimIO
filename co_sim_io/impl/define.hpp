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

#ifndef CO_SIM_IO_DEFINE_INCLUDED
#define CO_SIM_IO_DEFINE_INCLUDED

// System includes
#include <cstddef>
#include <memory>
#include <array>

namespace CoSimIO {

// signed integer type, 32 bit in 32 bit systems, but 64bit in 64 bit systems => like std::size_t but signed
using IdType = std::ptrdiff_t;

using CoordinatesType = std::array<double,3>;

enum class ControlSignal
{
    Dummy,
    BreakSolutionLoop,
    ConvergenceAchieved,

    AdvanceInTime,
    InitializeSolutionStep,
    Predict,
    SolveSolutionStep,
    FinalizeSolutionStep,
    OutputSolutionStep,

    ImportGeometry,
    ExportGeometry,
    ImportMesh,
    ExportMesh,
    ImportData,
    ExportData,
};

enum ConnectionStatus
{
    NotConnected,
    Connected,
    Disconnected,
    ConnectionError,
    DisconnectionError
};

// see https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf ; figure 2
enum ElementType
{
    VERTEX     = 1,
    LINE       = 3,
    TRIANGLE   = 5,
    QUAD       = 9,
    TETRA      = 10,
    HEXAHEDRON = 12
};

// Note: std::make_unique is C++14, this can be updated once we upgrade from C++11
template<typename C, typename...Args>
std::unique_ptr<C> make_unique(Args &&...args) {
    return std::unique_ptr<C>(new C(std::forward<Args>(args)...));
}

} //namespace CoSimIO

#endif // CO_SIM_IO_DEFINE_INCLUDED
