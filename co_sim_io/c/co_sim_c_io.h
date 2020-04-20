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

#ifndef KRATOS_CO_SIM_C_IO_H_INCLUDED
#define KRATOS_CO_SIM_C_IO_H_INCLUDED

// C Interface for CoSimulation
// see "co_sim_io.h"

#include "co_sim_io_c_info.h"

#ifdef __cplusplus
extern "C" { // Define extern C if C++ compiler is used
#endif

struct CoSimIO_ReturnInfo CoSimIO_Connect(
    const char* pConnectionName,
    const char* pSettingsFileName);

struct CoSimIO_ReturnInfo CoSimIO_Disconnect(
    const char* pConnectionName);

struct CoSimIO_ReturnInfo CoSimIO_ImportData(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pSize,
    double** ppData);

struct CoSimIO_ReturnInfo CoSimIO_ExportData(
    const char* pConnectionName,
    const char* pIdentifier,
    const int Size,
    const double** pData);

struct CoSimIO_ReturnInfo CoSimIO_ImportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pNumberOfNodes,
    int* pNumberOfElements,
    double** ppNodalCoordinates,
    int** ppElementConnectivities,
    int** ppElementTypes);

struct CoSimIO_ReturnInfo CoSimIO_ExportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    const int NumberOfNodes,
    const int NumberOfElements,
    const double** pNodalCoordinates,
    const int** pElementConnectivities,
    const int** pElementTypes);

struct CoSimIO_ReturnInfo CoSimIO_RegisterAdvanceInTime(
    const char* pConnectionName,
    double (*pFunctionPointer)(double));

struct CoSimIO_ReturnInfo CoSimIO_RegisterSolvingFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)());

struct CoSimIO_ReturnInfo CoSimIO_RegisterDataExchangeFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)(const char*, const char*));

struct CoSimIO_ReturnInfo CoSimIO_Run(const char* pConnectionName);

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

#endif // KRATOS_CO_SIM_C_IO_H_INCLUDED
