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
#include <memory>
#include <string>
#include <unordered_map>

namespace CoSimIO {

using SettingsType = std::unordered_map<std::string, std::string>;

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

// Note: std::make_unique is C++14, this can be updated once we upgrade from C++11
template<typename C, typename...Args>
std::unique_ptr<C> make_unique(Args &&...args) {
    return std::unique_ptr<C>(new C(std::forward<Args>(args)...));
}

} //namespace CoSimIO

#endif // CO_SIM_IO_DEFINE_INCLUDED