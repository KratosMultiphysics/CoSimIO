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

CoSimIO_Info CoSimIO_ImportMeshNEW(
    const CoSimIO_Info I_Info,
    CoSimIO_ModelPart O_ModelPart)
{
    CoSimIO::ModelPart& cpp_model_part = *(static_cast<CoSimIO::ModelPart*>(O_ModelPart.PtrCppModelPart));

    return ConvertInfo(CoSimIO::ImportMesh(ConvertInfo(I_Info), cpp_model_part));
}

CoSimIO_Info CoSimIO_ExportMeshNEW(
    const CoSimIO_Info I_Info,
    const CoSimIO_ModelPart I_ModelPart)
{
    const CoSimIO::ModelPart& cpp_model_part = *(static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart));

    return ConvertInfo(CoSimIO::ExportMesh(ConvertInfo(I_Info), cpp_model_part));
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
    constexpr int coordinates_per_node = 3;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemory<double>(I_NodalCoordinates, *I_NumberOfNodes));
    std::unique_ptr<DataContainer<int>> p_container_conn(new DataContainerRawMemory<int>(I_ElementConnectivities, *I_NumberOfElementConnectivities));
    std::unique_ptr<DataContainer<int>> p_container_types(new DataContainerRawMemory<int>(I_ElementTypes, *I_NumberOfElements));
    auto info = ConvertInfo(CoSimIO::ImportMesh(ConvertInfo(I_Info), *p_container_coords, *p_container_conn, *p_container_types));
    *I_NumberOfNodes = static_cast<int>(p_container_coords->size()) / coordinates_per_node;
    *I_NumberOfElementConnectivities = static_cast<int>(p_container_conn->size());
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
    constexpr int coordinates_per_node = 3;
    std::unique_ptr<DataContainer<double>> p_container_coords(new DataContainerRawMemoryReadOnly<double>(O_NodalCoordinates, O_NumberOfNodes * coordinates_per_node));
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


// Info functions
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


// ModelPart functions
CoSimIO_ModelPart CoSimIO_CreateModelPart(const char* I_Name)
{
    CoSimIO_ModelPart model_part;
    model_part.PtrCppModelPart = new CoSimIO::ModelPart(I_Name);
    return model_part;
}


const char* CoSimIO_ModelPart_Name(CoSimIO_ModelPart I_ModelPart)
{
    return static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->Name().c_str();
}

int CoSimIO_ModelPart_NumberOfNodes(CoSimIO_ModelPart I_ModelPart)
{
    return static_cast<int>(static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->NumberOfNodes());
}

int CoSimIO_ModelPart_NumberOfElements(CoSimIO_ModelPart I_ModelPart)
{
    return static_cast<int>(static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->NumberOfElements());
}


CoSimIO_Node CoSimIO_ModelPart_CreateNewNode(
    CoSimIO_ModelPart I_ModelPart,
    const int I_Id,
    const double I_X,
    const double I_Y,
    const double I_Z)
{
    CoSimIO::Node& cpp_node = static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->CreateNewNode(I_Id, I_X, I_Y, I_Z);
    CoSimIO_Node node;
    node.PtrCppNode = &cpp_node;
    return node;
}

CoSimIO_Element CoSimIO_ModelPart_CreateNewElement(
    CoSimIO_ModelPart I_ModelPart,
    const int I_Id,
    const CoSimIO_ElementType I_Type,
    const int* I_Connectivities)
{
    const CoSimIO::ElementType cpp_element_type = static_cast<CoSimIO::ElementType>(I_Type);
    const std::size_t num_nodes = CoSimIO::Internals::GetNumberOfNodesForElementType(cpp_element_type);
    // convert the C-connectivities to C++ connectivities
    CoSimIO::ConnectivitiesType connectivities(I_Connectivities, I_Connectivities+num_nodes);

    CoSimIO::Element& cpp_elem = static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->CreateNewElement(I_Id, cpp_element_type, connectivities);
    CoSimIO_Element elem;
    elem.PtrCppElement = &cpp_elem;
    return elem;
}


int CoSimIO_Node_Id(CoSimIO_Node I_Node)
{
    return static_cast<int>(static_cast<CoSimIO::Node*>(I_Node.PtrCppNode)->Id());
}

double CoSimIO_Node_X(CoSimIO_Node I_Node)
{
    return static_cast<CoSimIO::Node*>(I_Node.PtrCppNode)->X();
}

double CoSimIO_Node_Y(CoSimIO_Node I_Node)
{
    return static_cast<CoSimIO::Node*>(I_Node.PtrCppNode)->Y();
}

double CoSimIO_Node_Z(CoSimIO_Node I_Node)
{
    return static_cast<CoSimIO::Node*>(I_Node.PtrCppNode)->Z();
}

double CoSimIO_Node_Coordinate(CoSimIO_Node I_Node, const int I_Index)
{
    // add debug error if I_Index is out of bound (admissible values: 0,1,2)
    return static_cast<CoSimIO::Node*>(I_Node.PtrCppNode)->Coordinates()[I_Index];
}


CoSimIO_Node CoSimIO_ModelPart_GetNodeByIndex(CoSimIO_ModelPart I_ModelPart, const int I_Index)
{
    CoSimIO::Node& cpp_node = **(static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->NodesBegin()+I_Index);
    CoSimIO_Node node;
    node.PtrCppNode = &cpp_node;
    return node;
}

CoSimIO_Node CoSimIO_ModelPart_GetNodeById(CoSimIO_ModelPart I_ModelPart, const int I_Id)
{
    CoSimIO::Node& cpp_node = static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->GetNode(I_Id);
    CoSimIO_Node node;
    node.PtrCppNode = &cpp_node;
    return node;
}


int CoSimIO_Element_Id(CoSimIO_Element I_Element)
{
    return static_cast<int>(static_cast<CoSimIO::Element*>(I_Element.PtrCppElement)->Id());
}

CoSimIO_ElementType CoSimIO_Element_Type(CoSimIO_Element I_Element)
{
    return static_cast<CoSimIO_ElementType>(static_cast<CoSimIO::Element*>(I_Element.PtrCppElement)->Type());
}

int CoSimIO_Element_NumberOfNodes(CoSimIO_Element I_Element)
{
    return static_cast<int>(static_cast<CoSimIO::Element*>(I_Element.PtrCppElement)->NumberOfNodes());
}

CoSimIO_Node CoSimIO_Element_GetNodeByIndex(CoSimIO_Element I_Element, const int I_Index)
{
    CoSimIO::Node& cpp_node = **(static_cast<CoSimIO::Element*>(I_Element.PtrCppElement)->NodesBegin()+I_Index);
    CoSimIO_Node node;
    node.PtrCppNode = &cpp_node;
    return node;
}



CoSimIO_Element CoSimIO_ModelPart_GetElementByIndex(CoSimIO_ModelPart I_ModelPart, const int I_Index)
{
    CoSimIO::Element& cpp_elem = **(static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->ElementsBegin()+I_Index);
    CoSimIO_Element elem;
    elem.PtrCppElement = &cpp_elem;
    return elem;
}

CoSimIO_Element CoSimIO_ModelPart_GetElementById(CoSimIO_ModelPart I_ModelPart, const int I_Id)
{
    CoSimIO::Element& cpp_elem = static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart)->GetElement(I_Id);
    CoSimIO_Element elem;
    elem.PtrCppElement = &cpp_elem;
    return elem;
}


int CoSimIO_FreeModelPart(CoSimIO_ModelPart I_ModelPart)
{
    delete static_cast<CoSimIO::ModelPart*>(I_ModelPart.PtrCppModelPart);
    return 0;
}


void* CoSimIO_Malloc(size_t size){
    // Todo: Add a register of allocated memory: [pointer, size]
    return malloc(size);
}

void CoSimIO_Free (void* ptr){
    // Removing it from registered allocated memory
    free(ptr);
}
