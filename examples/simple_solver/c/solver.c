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

#include <stdio.h>
#include <stdlib.h>
#include "co_sim_io_c.h"

void ImportData(const char* pConnectionName, const char* pIdentifier)
{
    int incoming_data_size = 1;

    double**
    values_raw = (double**)malloc(sizeof(double*)*1);
    values_raw[0]=(double*)malloc(sizeof(double)*incoming_data_size);

    CoSimIO_ImportData(pConnectionName, pIdentifier, &incoming_data_size, values_raw);

    free(values_raw[1]);
    free(values_raw);
}

void ExportData(const char* pConnectionName, const char* pIdentifier)
{

}

void ImportMesh(const char* pConnectionName, const char* pIdentifier)
{

}

void ExportMesh(const char* pConnectionName, const char* pIdentifier)
{

}

void AdvanceInTime(CoSimIO_Info* pSolutionInfo)
{
    // return pCurrentTime + 0.1;
}

void InitializeSolutionStep(CoSimIO_Info* pSolutionInfo)
{

}

void SolveSolutionStep(CoSimIO_Info* pSolutionInfo)
{

}

void FinalizeSolutionStep(CoSimIO_Info* pSolutionInfo)
{

}


int main()
{
    CoSimIO_Connect("aaa", "ccc");

    // CoSimIO_RegisterSolvingFunction("aaa", "AdvanceInTime", &AdvanceInTime);
    // CoSimIO_RegisterSolvingFunction("aaa", "InitializeSolutionStep", &InitializeSolutionStep);
    // CoSimIO_RegisterSolvingFunction("aaa", "SolveSolutionStep",      &SolveSolutionStep);
    // CoSimIO_RegisterSolvingFunction("aaa", "FinalizeSolutionStep",   &FinalizeSolutionStep);

    // CoSimIO_RegisterDataExchangeFunction("aaa", "ImportData", &ImportData);
    // CoSimIO_RegisterDataExchangeFunction("aaa", "ExportData", &ExportData);
    // CoSimIO_RegisterDataExchangeFunction("aaa", "ImportMesh", &ImportMesh);
    // CoSimIO_RegisterDataExchangeFunction("aaa", "ExportMesh", &ExportMesh);

    CoSimIO_Run("aaa");

    CoSimIO_Disconnect("aaa");

    // printf() displays the string inside quotation
    printf("Hello, World!");
    return 0;
}