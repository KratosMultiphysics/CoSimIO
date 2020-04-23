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

// Project includes
extern "C" {
#include "co_sim_io_c.h"
}
#include "../co_sim_io.h"

// #define CreateGetValueInt(CoSimIO_SolutionInfo CInfo, const char* Key)
// {
//     return static_cast<CoSimIO::SolutionInfo>(CInfo.cpp_info_ptr).GetValue<int>(Key);
// }


namespace {
    CoSimIO_ReturnInfo ConvertInfo(CoSimIO::ReturnInfo yyy) {
        CoSimIO_ReturnInfo nnn;
        return nnn;
    }

    int GetValueInt(CoSimIO_SolutionInfo CInfo, const char* Key)
    {
        // return static_cast<CoSimIO::SolutionInfo>(CInfo.cpp_info_ptr).GetValue<int>(Key);
    }

    CoSimIO_SolutionInfo ConvertInfo(CoSimIO::SolutionInfo yyy) {
        CoSimIO_SolutionInfo nnn;
        // nnn.EchoLevel = yyy.GetValue<int>("EchoLevel");
        return nnn;
    }

    CoSimIO::SolutionInfo ConvertInfo(CoSimIO_SolutionInfo yyy) {
        CoSimIO::SolutionInfo nnn;
        return nnn;
    }

    CoSimIO_TransferInfo ConvertInfo(CoSimIO::TransferInfo yyy) {
        CoSimIO_TransferInfo nnn;
        return nnn;
    }

    CoSimIO::TransferInfo ConvertInfo(CoSimIO_TransferInfo yyy) {
        CoSimIO::TransferInfo nnn;
        return nnn;
    }
}


CoSimIO_ReturnInfo CoSimIO_Connect(const char* pConnectionName, const char* pSettingsFileName)
{
    return ConvertInfo(CoSimIO::Connect(pConnectionName, pSettingsFileName));
}

CoSimIO_ReturnInfo CoSimIO_Disconnect(const char* pConnectionName)
{
    return ConvertInfo(CoSimIO::Disconnect(pConnectionName));
}

CoSimIO_ReturnInfo CoSimIO_ImportData(
    const char* pConnectionName,
    const char* pIdentifier,
    int* pSize,
    double** ppData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemory<double>(ppData, *pSize));
    auto info = ConvertInfo(CoSimIO::ImportData(pConnectionName, pIdentifier, *p_container));
    *pSize = p_container->size();
    return info;
}

CoSimIO_ReturnInfo CoSimIO_ExportData(
    const char* pConnectionName,
    const char* pIdentifier,
    const int Size,
    const double** pData)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemoryReadOnly<double>(pData, Size));
    return ConvertInfo(CoSimIO::ExportData(pConnectionName, pIdentifier, *p_container));
}

CoSimIO_ReturnInfo CoSimIO_ImportMesh(
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
    *pNumberOfNodes = p_container_coords->size();
    *pNumberOfElements = p_container_types->size();
    return info;
}

CoSimIO_ReturnInfo CoSimIO_ExportMesh(
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

CoSimIO_ReturnInfo CoSimIO_ImportSolutionInfo(
    const char* pConnectionName,
    CoSimIO_SolutionInfo* pSolutionInfo)
{
    // // TODO: check if the conversions are working
    // CoSimIO::SolutionInfo tmp_info = ConvertInfo(rSolutionInfo);
    // CoSimIO::ImportSolutionInfo(pConnectionName, tmp_info);
    // rSolutionInfo = ConvertInfo(tmp_info);
}

CoSimIO_ReturnInfo CoSimIO_ExportSolutionInfo(
    const char* pConnectionName,
    const CoSimIO_SolutionInfo SolutionInfo)
{
    CoSimIO::ExportSolutionInfo(pConnectionName, ConvertInfo(SolutionInfo));
}

CoSimIO_ReturnInfo CoSimIO_RegisterSolvingFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)(CoSimIO_SolutionInfo*))
{
    // TODO use lambdas to do conversion of types, like done in other places!
    // return ConvertInfo(CoSimIO::Register(pConnectionName, pFunctionName, pFunctionPointer));
}

CoSimIO_ReturnInfo CoSimIO_RegisterDataExchangeFunction(
    const char* pConnectionName,
    const char* pFunctionName,
    void (*pFunctionPointer)(const char*, const char*))
{
    // TODO use lambdas to do conversion of types, like done in other places!
    // return ConvertInfo(CoSimIO::Register(pConnectionName, pFunctionName, pFunctionPointer));
}

CoSimIO_ReturnInfo CoSimIO_Run(const char* pConnectionName)
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
