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

// Project includes
extern "C" {
#include "co_sim_io_c.h"
}
#include "../co_sim_io.h"

// #define CreateGetValueInt(CoSimIO_Info CInfo, const char* Key)
// {
//     return static_cast<CoSimIO::SolutionInfo>(CInfo.cpp_info_ptr).GetValue<int>(Key);
// }


namespace {
    CoSimIO_Info ConvertInfo(CoSimIO::Info yyy) {
        CoSimIO_Info nnn;
        return nnn;
    }

    CoSimIO::Info ConvertInfo(CoSimIO_Info yyy) {
        CoSimIO::Info nnn;
        return nnn;
    }

    int GetValueInt(CoSimIO_Info CInfo, const char* Key)
    {
        // return static_cast<CoSimIO::SolutionInfo>(CInfo.cpp_info_ptr).GetValue<int>(Key);
        return 0;
    }
}


CoSimIO_Info CoSimIO_Connect(const char* pConnectionName, const char* pSettingsFileName)
{
    return ConvertInfo(CoSimIO::Connect(pConnectionName, pSettingsFileName));
}

CoSimIO_Info CoSimIO_Disconnect(const char* pConnectionName)
{
    return ConvertInfo(CoSimIO::Disconnect(pConnectionName));
}

CoSimIO_Info CoSimIO_ImportData(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pSize,
    double** ppData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemory<double>(ppData, *pSize));
    auto info = ConvertInfo(CoSimIO::ImportData(pConnectionName, pIdentifier, *p_container));
    *pSize = static_cast<int>(p_container->size());
    return info;
}

CoSimIO_Info CoSimIO_ExportData(
    const char* pConnectionName,
    const char* pIdentifier,
    const int Size,
    const double** pData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemoryReadOnly<double>(pData, Size));
    return ConvertInfo(CoSimIO::ExportData(pConnectionName, pIdentifier, *p_container));
}

CoSimIO_Info CoSimIO_ImportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pNumberOfNodes,
    int* pNumberOfElements,
    double** ppNodalCoordinates,
    int** ppElementConnectivities,
    int** ppElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemory<double>(ppNodalCoordinates, *pNumberOfNodes));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerRawMemory<int>(ppElementConnectivities, *pNumberOfElements)); // using "NumberOfElements" here is wrong! => has to be computed! Or sth like this ... (maybe even has to be passed...)
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerRawMemory<int>(ppElementTypes, *pNumberOfElements));
    auto info = ConvertInfo(CoSimIO::ImportMesh(pConnectionName, pIdentifier, *p_container_coords, *p_container_conn, *p_container_types));
    *pNumberOfNodes = static_cast<int>(p_container_coords->size());
    *pNumberOfElements = static_cast<int>(p_container_types->size());
    return info;
}

CoSimIO_Info CoSimIO_ExportMesh(
    const char* pConnectionName,
    const char* pIdentifier,
    const int NumberOfNodes,
    const int NumberOfElements,
    const double** pNodalCoordinates,
    const int** pElementConnectivities,
    const int** pElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemoryReadOnly<double>(pNodalCoordinates, NumberOfNodes));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerRawMemoryReadOnly<int>(pElementConnectivities, NumberOfElements)); // using "NumberOfElements" here is wrong! => has to be computed! Or sth like this ... (maybe even has to be passed...)
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerRawMemoryReadOnly<int>(pElementTypes, NumberOfElements));
    return ConvertInfo(CoSimIO::ExportMesh(pConnectionName, pIdentifier, *p_container_coords, *p_container_conn, *p_container_types));
}

CoSimIO_Info CoSimIO_ImportInfo(
    const char* pConnectionName,
    CoSimIO_Info* pSolutionInfo)
{
    // // TODO: check if the conversions are working
    // CoSimIO::SolutionInfo tmp_info = ConvertInfo(rSolutionInfo);
    // CoSimIO::ImportSolutionInfo(pConnectionName, tmp_info);
    // rSolutionInfo = ConvertInfo(tmp_info);
    CoSimIO_Info aaa;
    return aaa;
}

CoSimIO_Info CoSimIO_ExportInfo(
    const char* pConnectionName,
    const CoSimIO_Info SolutionInfo)
{
    // CoSimIO::ExportInfo(pConnectionName, ConvertInfo(SolutionInfo));
    CoSimIO_Info aaa;
    return aaa;
}

CoSimIO_Info CoSimIO_Register(
    const char* I_ConnectionName,
    const char* I_FunctionName,
    double (*FunctionPointer)(CoSimIO_Info*))
{
    // TODO use lambdas to do conversion of types, like done in other places!
    // return ConvertInfo(CoSimIO::Register(I_ConnectionName, I_FunctionName, pFunctionPointer));
    CoSimIO_Info aaa;
    return aaa;
}

CoSimIO_Info CoSimIO_Run(const char* pConnectionName)
{
    return ConvertInfo(CoSimIO::Run(pConnectionName));
}

int CoSimIO_IsConverged(const char* pConnectionName)
{
    return CoSimIO::IsConverged(pConnectionName);
}



void _AllocateMemoryInt(const int* pSize, int** ppData)
{
    free(*ppData); // making sure that potenetially allocated memory is freed. This is ok also if nothing is allocated aka NULL
    *ppData = (int *)malloc((*pSize)*sizeof(int));

    if (!(*ppData)) {
        printf("ERROR, memory allocation (int) failed!");
        exit(0);
    }
}

void _AllocateMemoryDouble(const int Size, double** ppData)
{
    free(*ppData); // making sure that potenetially allocated memory is freed. This is ok also if nothing is allocated aka NULL
    *ppData = (double *)malloc((Size)*sizeof(double));

    if (!(*ppData)) {
        printf("ERROR, memory allocation (double) failed!");
        exit(0);
    }
}

void _FreeMemory(void** ppData)
{
    free(*ppData);
}
