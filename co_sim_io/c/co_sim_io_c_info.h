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

#ifndef CO_SIM_IO_C_INFO_H_INCLUDED
#define CO_SIM_IO_C_INFO_H_INCLUDED

// System includes

#ifdef CO_SIM_IO_USING_MPI
#include "mpi.h"
#endif // CO_SIM_IO_USING_MPI

// #define CreateNewGetValue(type) \
// type ##type,  const char* Key)

// #define DefineNewGetValue(type) \
// type GetValue##type(char* label, CoSimIO_TransferInfo);

typedef struct CoSimIO_SolutionInfo
{
    void* PtrCppInfo;
} CoSimIO_SolutionInfo;

typedef struct CoSimIO_TransferInfo
{
} CoSimIO_TransferInfo;

typedef struct CoSimIO_ReturnInfo
{
    int ReturnCode;
} CoSimIO_ReturnInfo;

#endif // CO_SIM_IO_C_INFO_H_INCLUDED