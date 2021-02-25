/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Philipp Bucher (https://github.com/philbucher)
*/

/* Project includes */
#include "c/co_sim_io_c.h"

#include "../checks.h"

int main()
{
    /* declaring variables */
    int i;
    int connectivity[3];
    CoSimIO_Element elem_tri, elem_line, elem_point, elem_tri_by_index, elem_line_by_index, elem_point_by_index, elem_tri_by_id, elem_line_by_id, elem_point_by_id;
    CoSimIO_ModelPart model_part;

    model_part = CoSimIO_CreateModelPart("my_model_part");
    for (i=0; i<5; ++i) {
        CoSimIO_ModelPart_CreateNewNode(
            model_part,
            i+1,
            0.1*i,
            i+2,
            -i);
    }

    /* the memory of the element is managed on the C++ side
       create triangle element */
    connectivity[0] = 1;
    connectivity[1] = 2;
    connectivity[2] = 3;

    elem_tri = CoSimIO_ModelPart_CreateNewElement(
        model_part,
        2,
        CoSimIO_Triangle2D3,
        connectivity);

    /* create line element */
    connectivity[0] = 3;
    connectivity[1] = 4;

    elem_line = CoSimIO_ModelPart_CreateNewElement(
        model_part,
        5,
        CoSimIO_Line2D2,
        connectivity);

    /* create point element */
    connectivity[0] = 4;

    elem_point = CoSimIO_ModelPart_CreateNewElement(
        model_part,
        8,
        CoSimIO_Point2D,
        connectivity);


    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_Id(elem_tri));
    COSIMIO_CHECK_INT_EQUAL(5, CoSimIO_Element_Id(elem_line));
    COSIMIO_CHECK_INT_EQUAL(8, CoSimIO_Element_Id(elem_point));

    COSIMIO_CHECK_INT_EQUAL(3, CoSimIO_Element_NumberOfNodes(elem_tri));
    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_NumberOfNodes(elem_line));
    COSIMIO_CHECK_INT_EQUAL(1, CoSimIO_Element_NumberOfNodes(elem_point));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Triangle2D3, CoSimIO_Element_Type(elem_tri));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Line2D2, CoSimIO_Element_Type(elem_line));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Point2D, CoSimIO_Element_Type(elem_point));

    /* checking connectivities */
    for (i=0; i<CoSimIO_Element_NumberOfNodes(elem_tri); ++i) {
        COSIMIO_CHECK_INT_EQUAL(i+1, CoSimIO_Node_Id(CoSimIO_Element_GetNodeByIndex(elem_tri, i)));
    }

    for (i=0; i<CoSimIO_Element_NumberOfNodes(elem_line); ++i) {
        COSIMIO_CHECK_INT_EQUAL(i+3, CoSimIO_Node_Id(CoSimIO_Element_GetNodeByIndex(elem_line, i)));
    }

    COSIMIO_CHECK_INT_EQUAL(4, CoSimIO_Node_Id(CoSimIO_Element_GetNodeByIndex(elem_point, 0)));

    /* check element retrieval functions */
    elem_tri_by_index = CoSimIO_ModelPart_GetElementByIndex(model_part, 0);
    elem_line_by_index = CoSimIO_ModelPart_GetElementByIndex(model_part, 1);
    elem_point_by_index = CoSimIO_ModelPart_GetElementByIndex(model_part, 2);

    elem_tri_by_id = CoSimIO_ModelPart_GetElementById(model_part, 2);
    elem_line_by_id = CoSimIO_ModelPart_GetElementById(model_part, 5);
    elem_point_by_id = CoSimIO_ModelPart_GetElementById(model_part, 8);


    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_Id(elem_tri_by_index));
    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_Id(elem_tri_by_id));
    COSIMIO_CHECK_INT_EQUAL(5, CoSimIO_Element_Id(elem_line_by_index));
    COSIMIO_CHECK_INT_EQUAL(5, CoSimIO_Element_Id(elem_line_by_id));
    COSIMIO_CHECK_INT_EQUAL(8, CoSimIO_Element_Id(elem_point_by_index));
    COSIMIO_CHECK_INT_EQUAL(8, CoSimIO_Element_Id(elem_point_by_id));

    COSIMIO_CHECK_INT_EQUAL(3, CoSimIO_Element_NumberOfNodes(elem_tri_by_index));
    COSIMIO_CHECK_INT_EQUAL(3, CoSimIO_Element_NumberOfNodes(elem_tri_by_id));
    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_NumberOfNodes(elem_line_by_index));
    COSIMIO_CHECK_INT_EQUAL(2, CoSimIO_Element_NumberOfNodes(elem_line_by_id));
    COSIMIO_CHECK_INT_EQUAL(1, CoSimIO_Element_NumberOfNodes(elem_point_by_index));
    COSIMIO_CHECK_INT_EQUAL(1, CoSimIO_Element_NumberOfNodes(elem_point_by_id));

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Triangle2D3, CoSimIO_Element_Type(elem_tri_by_index));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Triangle2D3, CoSimIO_Element_Type(elem_tri_by_id));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Line2D2, CoSimIO_Element_Type(elem_line_by_index));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Line2D2, CoSimIO_Element_Type(elem_line_by_id));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Point2D, CoSimIO_Element_Type(elem_point_by_index));
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Point2D, CoSimIO_Element_Type(elem_point_by_id));

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
