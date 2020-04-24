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

#ifndef CO_SIM_IO_C_H_INCLUDED
#define CO_SIM_IO_C_H_INCLUDED

// C Interface for CoSimulation
// see "co_sim_io.hpp"

#include "co_sim_io_c_info.h"

#ifdef __cplusplus
extern "C" { // Define extern C if C++ compiler is used
#endif

CoSimIO_Info CoSimIO_Connect(
    const char* pConnectionName,
    const char* pSettingsFileName);

CoSimIO_Info CoSimIO_Disconnect(
    const char* pConnectionName);

CoSimIO_Info CoSimIO_ImportData(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pSize,
    double** ppData);

CoSimIO_Info CoSimIO_ExportData(
    const char* pConnectionName,
    const char* pIdentifier,
    const int I_Size,
    const double** pData);

CoSimIO_Info CoSimIO_ImportMesh(
    const char* I_ConnectionName,
    const char* pIdentifier,
    int* pNumberOfNodes,
    int* pNumberOfElements,
    double** O_NodalCoordinates,
    int** ppElementConnectivities,
    int** ppElementTypes);

CoSimIO_Info CoSimIO_ExportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    const int NumberOfNodes,
    const int NumberOfElements,
    const double** pNodalCoordinates,
    const int** pElementConnectivities,
    const int** pElementTypes);


CoSimIO_Info CoSimIO_ImportInfo(
    const char* pConnectionName,
    CoSimIO_Info* pSolutionInfo);

CoSimIO_Info CoSimIO_ExportInfo(
    const char* pConnectionName,
    const CoSimIO_Info SolutionInfo);

CoSimIO_Info CoSimIO_Register(
    const char* I_ConnectionName,
    const char* I_FunctionName,
    double (*FunctionPointer)(CoSimIO_Info*));

CoSimIO_Info CoSimIO_Run(const char* I_ConnectionName);

int CoSimIO_IsConverged(const char* I_ConnectionName);

// TODO refactor and move these functions to separate file, since they are only used in the fortran interface
// The following functions are intended to only be used from Fortran
// This is because memory used in the CoSimIO should be allocated and freed in C
void _AllocateMemoryInt(const int Size, int** ppData);
void _AllocateMemoryDouble(const int Size, double** ppData);
void _FreeMemory(void** ppData);

#ifdef __cplusplus
}
#endif

#endif // CO_SIM_IO_C_H_INCLUDED
