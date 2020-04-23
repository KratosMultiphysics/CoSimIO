// Kratos   ______     _____ _           ________
//         / ____/___ / ___/(_)___ ___  /  _/ __ |
//        / /   / __ \\__ \/ / __ `__ \ / // / / /
//       / /___/ /_/ /__/ / / / / / / // // /_/ /
//       \____/\____/____/_/_/ /_/ /_/___/\____/
//      Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef CO_SIM_IO_C_H_INCLUDED
#define CO_SIM_IO_C_H_INCLUDED

// C Interface for CoSimulation
// see "co_sim_io.h"

#include "co_sim_io_c_info.h"

#ifdef __cplusplus
extern "C" { // Define extern C if C++ compiler is used
#endif

CoSimIO_ReturnInfo CoSimIO_Connect(
    const char* pConnectionName,
    const char* pSettingsFileName);

CoSimIO_ReturnInfo CoSimIO_Disconnect(
    const char* pConnectionName);

CoSimIO_ReturnInfo CoSimIO_ImportData(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pSize,
    double** ppData);

CoSimIO_ReturnInfo CoSimIO_ExportData(
    const char* pConnectionName,
    const char* pIdentifier,
    const int I_Size,
    const double** pData);

CoSimIO_ReturnInfo CoSimIO_ImportMesh(
    const char* I_ConnectionName,
    const char* pIdentifier,
    int* pNumberOfNodes,
    int* pNumberOfElements,
    double** O_NodalCoordinates,
    int** ppElementConnectivities,
    int** ppElementTypes);

CoSimIO_ReturnInfo CoSimIO_ExportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    const int NumberOfNodes,
    const int NumberOfElements,
    const double** pNodalCoordinates,
    const int** pElementConnectivities,
    const int** pElementTypes);


CoSimIO_ReturnInfo CoSimIO_ImportSolutionInfo(
    const char* pConnectionName,
    CoSimIO_SolutionInfo* pSolutionInfo);

CoSimIO_ReturnInfo CoSimIO_ExportSolutionInfo(
    const char* pConnectionName,
    const CoSimIO_SolutionInfo SolutionInfo);

CoSimIO_ReturnInfo CoSimIO_RegisterAdvanceInTime(
    const char* pConnectionName,
    double (*pFunctionPointer)(double));

CoSimIO_ReturnInfo CoSimIO_RegisterSolvingFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)(CoSimIO_SolutionInfo*));

CoSimIO_ReturnInfo CoSimIO_RegisterDataExchangeFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)(const char*, const char*));

CoSimIO_ReturnInfo CoSimIO_Run(const char* pConnectionName);

int CoSimIO_IsConverged(const char* pConnectionName);

// TODO refactor and move these functions to separate file, sicne they are only used in the fortran interface
// The following functions are intended to only be used from Fortran
// This is because memory used in the CoSimIO should be allocated and freed in C
void _AllocateMemoryInt(const int Size, int** ppData);
void _AllocateMemoryDouble(const int Size, double** ppData);
void _FreeMemory(void** ppData);

#ifdef __cplusplus
}
#endif

#endif // CO_SIM_IO_C_H_INCLUDED
