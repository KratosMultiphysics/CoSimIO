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
    void* PtrCppNode;
} CoSimIO_Node;

typedef struct CoSimIO_Element
{
    void* PtrCppElement;
} CoSimIO_Element;

typedef struct CoSimIO_ModelPart
{
    void* PtrCppModelPart;
} CoSimIO_ModelPart;

typedef enum
{
    CoSimIO_Hexahedra3D20,
    CoSimIO_Hexahedra3D27,
    CoSimIO_Hexahedra3D8,
    CoSimIO_Prism3D15,
    CoSimIO_Prism3D6,
    CoSimIO_Quadrilateral2D4,
    CoSimIO_Quadrilateral2D8,
    CoSimIO_Quadrilateral2D9,
    CoSimIO_Quadrilateral3D4,
    CoSimIO_Quadrilateral3D8,
    CoSimIO_Quadrilateral3D9,
    CoSimIO_Tetrahedra3D10,
    CoSimIO_Tetrahedra3D4,
    CoSimIO_Triangle2D3,
    CoSimIO_Triangle2D6,
    CoSimIO_Triangle3D3,
    CoSimIO_Triangle3D6,
    CoSimIO_Line2D2,
    CoSimIO_Line2D3,
    CoSimIO_Line3D2,
    CoSimIO_Line3D3,
    CoSimIO_Point2D,
    CoSimIO_Point3D
} CoSimIO_ElementType;


// Node functions
int CoSimIO_Node_Id(CoSimIO_Node I_Node);
double CoSimIO_Node_X(CoSimIO_Node I_Node);
double CoSimIO_Node_Y(CoSimIO_Node I_Node);
double CoSimIO_Node_Z(CoSimIO_Node I_Node);
// double* CoSimIO_Coordinates(CoSimIO_Node I_Node);


// Element functions
int CoSimIO_Element_Id(CoSimIO_Element I_Element);
CoSimIO_ElementType CoSimIO_Element_Type(CoSimIO_Element I_Element);
int CoSimIO_Element_NumberOfNodes(CoSimIO_Element I_Element);
// CoSimIO_Node* CoSimIO_Element_Nodes(CoSimIO_Element I_Element);


// ModelPart functions
CoSimIO_ModelPart CoSimIO_CreateModelPart(const char* I_Name);

int CoSimIO_FreeModelPart(CoSimIO_ModelPart I_ModelPart);

const char* CoSimIO_ModelPart_Name(CoSimIO_ModelPart I_ModelPart);
int CoSimIO_ModelPart_NumberOfNodes(CoSimIO_ModelPart I_ModelPart);
int CoSimIO_ModelPart_NumberOfElements(CoSimIO_ModelPart I_ModelPart);
// CoSimIO_Node* CoSimIO_ModelPart_Nodes(CoSimIO_ModelPart I_ModelPart);
// CoSimIO_Element* CoSimIO_ModelPart_Elements(CoSimIO_ModelPart I_ModelPart);

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
