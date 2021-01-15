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

#ifndef CO_SIM_IO_C_MODEL_PART_INCLUDED
#define CO_SIM_IO_C_MODEL_PART_INCLUDED

typedef struct CoSimIO_Node
{
    void* PtrCppInfo;
} CoSimIO_Node;

typedef struct CoSimIO_Element
{
    void* PtrCppInfo;
} CoSimIO_Element;

typedef struct CoSimIO_ModelPart
{
    void* PtrCppInfo;
} CoSimIO_ModelPart;

// see https://vtk.org/wp-content/uploads/2015/04/file-formats.pdf ; figure 2
typedef enum
{
    CoSimIO_VERTEX     = 1,
    CoSimIO_LINE       = 3,
    CoSimIO_TRIANGLE   = 5,
    CoSimIO_QUAD       = 9,
    CoSimIO_TETRA      = 10,
    CoSimIO_HEXAHEDRON = 12
} CoSimIO_ElementType;

int CoSimIO_Node_Id(CoSimIO_Node I_Node);
double CoSimIO_Node_X(CoSimIO_Node I_Node);
double CoSimIO_Node_Y(CoSimIO_Node I_Node);
double CoSimIO_Node_Z(CoSimIO_Node I_Node);


int CoSimIO_Element_Id(CoSimIO_Element I_Element);
CoSimIO_ElementType CoSimIO_Element_Type(CoSimIO_Element I_Element);
int CoSimIO_Element_NumberOfNodes(CoSimIO_Element I_Element);


CoSimIO_ModelPart CoSimIO_CreateModelPart(const char* I_Name);

int CoSimIO_FreeModelPart(CoSimIO_ModelPart I_ModelPart);

const char* CoSimIO_ModelPart_Name(CoSimIO_ModelPart I_ModelPart);
int CoSimIO_ModelPart_NumberOfNodes(CoSimIO_ModelPart I_ModelPart);
int CoSimIO_ModelPart_NumberOfElements(CoSimIO_ModelPart I_ModelPart);

CoSimIO_Node CoSimIO_ModelPart_CreateNewNode(
    CoSimIO_ModelPart I_ModelPart,
    const int I_Id,
    const double I_X,
    const double I_Y,
    const double I_Z);

CoSimIO_Element CoSimIO_ModelPart_CreateNewElement(
    CoSimIO_ModelPart I_ModelPart,
    const int I_Id,
    const CoSimIO_ElementType I_Type,
    const int* I_Connectivities);


#endif // CO_SIM_IO_C_MODEL_PART_INCLUDED
