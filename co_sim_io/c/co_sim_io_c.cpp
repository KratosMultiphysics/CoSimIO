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
#include "../co_sim_io.hpp"

namespace {
    // get C Info from C++ Info
    CoSimIO_Info ConvertInfo(CoSimIO::Info I_Info) {
        CoSimIO_Info info;
        info.PtrCppInfo = new CoSimIO::Info(I_Info);
        return info;
    }

    // get C++ Info from C Info
    CoSimIO::Info ConvertInfo(CoSimIO_Info I_Info) {
        return CoSimIO::Info(*(static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)));
    }
}


CoSimIO_Info CoSimIO_Hello()
{
    return ConvertInfo(CoSimIO::Hello());
}

CoSimIO_Info CoSimIO_Connect(const CoSimIO_Info I_Settings)
{
    return ConvertInfo(CoSimIO::Connect(ConvertInfo(I_Settings)));
}

CoSimIO_Info CoSimIO_Disconnect(const CoSimIO_Info I_Info)
{
    return ConvertInfo(CoSimIO::Disconnect(ConvertInfo(I_Info)));
}

CoSimIO_Info CoSimIO_ImportData(
    const CoSimIO_Info I_Info,
    int* O_Size,
    double** O_Data)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemory<double>(O_Data, *O_Size));
    auto info = ConvertInfo(CoSimIO::ImportData(ConvertInfo(I_Info), *p_container));
    *O_Size = static_cast<int>(p_container->size());
    return info;
}

CoSimIO_Info CoSimIO_ExportData(
    const CoSimIO_Info I_Info,
    const int I_Size,
    const double* I_Data)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container(new DataContainerRawMemoryReadOnly<double>(I_Data, I_Size));
    return ConvertInfo(CoSimIO::ExportData(ConvertInfo(I_Info), *p_container));
}

CoSimIO_Info CoSimIO_ImportMesh(
    const CoSimIO_Info I_Info,
    int* I_NumberOfNodes,
    int* I_NumberOfElements,
    int* I_NumberOfElementConnectivities,
    double** I_NodalCoordinates,
    int** I_ElementConnectivities,
    int** I_ElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemory<double>(I_NodalCoordinates, *I_NumberOfNodes));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerRawMemory<int>(I_ElementConnectivities, *I_NumberOfElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerRawMemory<int>(I_ElementTypes, *I_NumberOfElements));
    auto info = ConvertInfo(CoSimIO::ImportMesh(ConvertInfo(I_Info), *p_container_coords, *p_container_conn, *p_container_types));
    *I_NumberOfNodes = static_cast<int>(p_container_coords->size());
    *I_NumberOfElements = static_cast<int>(p_container_types->size());
    return info;
}

CoSimIO_Info CoSimIO_ExportMesh(
    const CoSimIO_Info I_Info,
    const int O_NumberOfNodes,
    const int O_NumberOfElements,
    const int O_NumberOfElementConnectivities,
    const double* O_NodalCoordinates,
    const int* O_ElementConnectivities,
    const int* O_ElementTypes)
{
    using namespace CoSimIO::Internals;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemoryReadOnly<double>(O_NodalCoordinates, O_NumberOfNodes));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerRawMemoryReadOnly<int>(O_ElementConnectivities, O_NumberOfElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerRawMemoryReadOnly<int>(O_ElementTypes, O_NumberOfElements));
    return ConvertInfo(CoSimIO::ExportMesh(ConvertInfo(I_Info), *p_container_coords, *p_container_conn, *p_container_types));
}

void CoSimIO_PrintInfo(FILE *Stream,
    const CoSimIO_Info I_Info)
{
    std::stringstream buffer;
    buffer << *static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo) << std::endl;
    fprintf(Stream, "%s", buffer.str().c_str());
}

CoSimIO_Info CoSimIO_ImportInfo(
    const CoSimIO_Info I_Info)
{
    return ConvertInfo(CoSimIO::ImportInfo(ConvertInfo(I_Info)));
}

CoSimIO_Info CoSimIO_ExportInfo(
    const CoSimIO_Info I_Info)
{
    return ConvertInfo(CoSimIO::ExportInfo(ConvertInfo(I_Info)));
}

CoSimIO_Info CoSimIO_Register(
    const CoSimIO_Info I_Info,
    CoSimIO_Info (*I_FunctionPointer)(const CoSimIO_Info I_Info))
{
    using FunctionPointerType = CoSimIO::Internals::Connection::FunctionPointerType;

    // auto return does not work here!
    FunctionPointerType fct_callback = [I_FunctionPointer](const CoSimIO::Info& I_Info)
    {
        return ConvertInfo(I_FunctionPointer(ConvertInfo(I_Info)));
    };

    return ConvertInfo(CoSimIO::Register(ConvertInfo(I_Info), fct_callback));
}


CoSimIO_Info CoSimIO_Run(const CoSimIO_Info I_Info)
{
    return ConvertInfo(CoSimIO::Run(ConvertInfo(I_Info)));
}

CoSimIO_Info CoSimIO_IsConverged(const CoSimIO_Info I_Info)
{
    return ConvertInfo(CoSimIO::IsConverged(ConvertInfo(I_Info)));
}


CoSimIO_Info CoSimIO_CreateInfo()
{
    CoSimIO_Info info;
    info.PtrCppInfo = new CoSimIO::Info();
    return info;
}

int CoSimIO_FreeInfo(CoSimIO_Info I_Info)
{
    delete static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo);

    return 0;
}

int CoSimIO_Info_Has(const CoSimIO_Info I_Info, const char* I_Key)
{
    return static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Has(I_Key);
}

void CoSimIO_Info_Erase(const CoSimIO_Info I_Info, const char* I_Key)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Erase(I_Key);
}

void CoSimIO_Info_Clear(const CoSimIO_Info I_Info)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Clear();
}
int CoSimIO_Info_Size(const CoSimIO_Info I_Info)
{
    return static_cast<int>(static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Size());
}


int CoSimIO_Info_GetInt(const CoSimIO_Info I_Info, const char* I_Key)
{
    return static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Get<int>(I_Key);
}

double CoSimIO_Info_GetDouble(const CoSimIO_Info I_Info, const char* I_Key)
{
    return static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Get<double>(I_Key);
}

int CoSimIO_Info_GetBool(const CoSimIO_Info I_Info, const char* I_Key)
{
    return static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Get<bool>(I_Key);
}

const char* CoSimIO_Info_GetString(const CoSimIO_Info I_Info, const char* I_Key)
{
    return static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Get<std::string>(I_Key).c_str();
}


void CoSimIO_Info_SetInt(CoSimIO_Info I_Info, const char* I_Key, const int I_Value)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Set<int>(I_Key, I_Value);
}

void CoSimIO_Info_SetDouble(CoSimIO_Info I_Info, const char* I_Key, const double I_Value)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Set<double>(I_Key, I_Value);
}

void CoSimIO_Info_SetBool(CoSimIO_Info I_Info, const char* I_Key, const int I_Value)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Set<bool>(I_Key, I_Value);
}

void CoSimIO_Info_SetString(CoSimIO_Info I_Info, const char* I_Key, const char* I_Value)
{
    static_cast<CoSimIO::Info*>(I_Info.PtrCppInfo)->Set<std::string>(I_Key, I_Value);
}

void* CoSimIO_Malloc(size_t size){
    // Todo: Add a register of allocated memory: [pointer, size]
    return malloc(size);
}

void CoSimIO_Free (void* ptr){
    // Removing it from registered allocated memory
    free(ptr);
}
