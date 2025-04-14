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

int main(int argc, const char** argv)
{
    COSIMIO_UNUSED(argc);
    COSIMIO_UNUSED(argv);

    /* declaring variables */
    int i;
    int connectivity[2];
    CoSimIO_Node node;
    CoSimIO_Element elem;
    CoSimIO_ModelPart model_part;

    model_part = CoSimIO_CreateModelPart("my_model_part");

    COSIMIO_CHECK_STRING_EQUAL(CoSimIO_ModelPart_Name(model_part), "my_model_part");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (i=0; i<4; ++i) {
        CoSimIO_ModelPart_CreateNewNode(
            model_part,
            i+1,
            0.1*i,
            i+2,
            -i);
    }
    for (i=4; i<7; ++i) {
        CoSimIO_ModelPart_CreateNewGhostNode(
            model_part,
            i+1,
            0.1*i,
            i+2,
            -i,
            123);
    }

    /* iterate the nodes */
    for (i=0; i<CoSimIO_ModelPart_NumberOfNodes(model_part); ++i) {
        node = CoSimIO_ModelPart_GetNodeByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), i+1);
    }

    /* iterate the local nodes */
    for (i=0; i<CoSimIO_ModelPart_NumberOfLocalNodes(model_part); ++i) {
        node = CoSimIO_ModelPart_GetLocalNodeByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), i+1);
    }

    /* iterate the ghost nodes */
    for (i=0; i<CoSimIO_ModelPart_NumberOfGhostNodes(model_part); ++i) {
        node = CoSimIO_ModelPart_GetGhostNodeByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), i+5);
    }

    /* get a specific node by Id */
    node = CoSimIO_ModelPart_GetNodeById(model_part, 3);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), 3);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 7);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), 4);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), 3);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (i=0; i<2; ++i) {
        connectivity[0] = i+1;
        connectivity[1] = i+4;
        CoSimIO_ModelPart_CreateNewElement(
            model_part,
            i+1,
            CoSimIO_Line2D2,
            connectivity);
    }

    /* iterate the elements */
    for (i=0; i<CoSimIO_ModelPart_NumberOfElements(model_part); ++i) {
        elem = CoSimIO_ModelPart_GetElementByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Element_Id(elem), i+1)
    }

    /* get a specific element by Id */
    elem = CoSimIO_ModelPart_GetElementById(model_part, 1);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_Element_Id(elem), 1);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 7);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 2);

    /* remove all Nodes and Elements */
    CoSimIO_ModelPart_Clear(model_part);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
