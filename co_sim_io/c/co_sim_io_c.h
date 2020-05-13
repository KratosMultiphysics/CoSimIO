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

#ifndef CO_SIM_IO_C_INCLUDED
#define CO_SIM_IO_C_INCLUDED

// C Interface for CoSimulation
// see "co_sim_io.hpp"

#include "co_sim_io_c_info.h"

#ifdef __cplusplus
extern "C" { // Define extern C if C++ compiler is used
#endif

CoSimIO_Info CoSimIO_Hello();

CoSimIO_Info CoSimIO_Connect(
    const CoSimIO_Info I_Settings);

CoSimIO_Info CoSimIO_Disconnect(
    const CoSimIO_Info I_Info);

CoSimIO_Info CoSimIO_ImportData(
    const CoSimIO_Info I_Info,
    int* pSize,
    double** ppData);

CoSimIO_Info CoSimIO_ExportData(
    const CoSimIO_Info* I_Info,
    const int I_Size,
    const double** pData);

CoSimIO_Info CoSimIO_ImportMesh(
    const CoSimIO_Info I_Info,
    int* pNumberOfNodes,
    int* pNumberOfElements,
    double** O_NodalCoordinates,
    int** ppElementConnectivities,
    int** ppElementTypes);

CoSimIO_Info CoSimIO_ExportMesh(
    const CoSimIO_Info I_Info,
    const int NumberOfNodes,
    const int NumberOfElements,
    const double** pNodalCoordinates,
    const int** pElementConnectivities,
    const int** pElementTypes);


CoSimIO_Info CoSimIO_ImportInfo(
    const CoSimIO_Info I_Info);

CoSimIO_Info CoSimIO_ExportInfo(
    const CoSimIO_Info I_Info);

CoSimIO_Info CoSimIO_Register(
    const CoSimIO_Info I_Info,
    CoSimIO_Info (*I_FunctionPointer)(const CoSimIO_Info I_Info));

CoSimIO_Info CoSimIO_Run(const CoSimIO_Info I_Info);

CoSimIO_Info CoSimIO_IsConverged(const CoSimIO_Info I_Info);

#ifdef __cplusplus
}
#endif

#endif // CO_SIM_IO_C_INCLUDED
