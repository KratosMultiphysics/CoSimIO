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
    int j;
    int n_nodes;
    int conn_counter=0;
    #define NUM_LOCAL_NODES 20
    #define NUM_GHOST_NODES 15
    #define NUM_ELEMENTS 12
    int ids[NUM_LOCAL_NODES];
    int part_idx[NUM_LOCAL_NODES];
    double node_x[NUM_LOCAL_NODES];
    double node_y[NUM_LOCAL_NODES];
    double node_z[NUM_LOCAL_NODES];
    int ghost_node_ids[NUM_LOCAL_NODES]; /* allocating more to make handling easier */
    CoSimIO_ElementType types[NUM_ELEMENTS];
    int conn[NUM_ELEMENTS*3]; /*allocating more to be safe*/
    CoSimIO_ModelPart model_part;
    CoSimIO_Node node;
    CoSimIO_Element elem;

    model_part = CoSimIO_CreateModelPart("my_model_part");

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (i=0; i<NUM_LOCAL_NODES; ++i) {
        ids[i] = i+1;
        ghost_node_ids[i] = NUM_LOCAL_NODES+1+i;
        node_x[i] = i*1.1;
        node_y[i] = i-123.589;
        node_z[i] = i*i;
        part_idx[i]=i;
    }

    for (i=0; i<NUM_ELEMENTS; ++i) {
        n_nodes = i%2==0 ? 2 : 3;
        types[i] = n_nodes==2 ? CoSimIO_Line2D2 : CoSimIO_Triangle3D3;

        for (j=0; j<n_nodes; ++j) {
            conn[conn_counter++] = i+j+1;
        }
    }

    CoSimIO_ModelPart_CreateNewNodes(model_part, NUM_LOCAL_NODES, ids, node_x, node_y, node_z);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), NUM_LOCAL_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), NUM_LOCAL_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (i=0; i<CoSimIO_ModelPart_NumberOfNodes(model_part); ++i) {
        node = CoSimIO_ModelPart_GetNodeByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), ids[i]);
    }

    CoSimIO_ModelPart_CreateNewGhostNodes(model_part, NUM_GHOST_NODES, ghost_node_ids, node_x, node_y, node_z, part_idx);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), NUM_LOCAL_NODES+NUM_GHOST_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), NUM_LOCAL_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), NUM_GHOST_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (i=0; i<CoSimIO_ModelPart_NumberOfGhostNodes(model_part); ++i) {
        node = CoSimIO_ModelPart_GetGhostNodeByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Node_Id(node), ghost_node_ids[i]);
    }

    CoSimIO_ModelPart_CreateNewElements(model_part, NUM_ELEMENTS, ids, types, conn_counter, conn);

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), NUM_LOCAL_NODES+NUM_GHOST_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfLocalNodes(model_part), NUM_LOCAL_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfGhostNodes(model_part), NUM_GHOST_NODES);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), NUM_ELEMENTS);

    for (i=0; i<CoSimIO_ModelPart_NumberOfElements(model_part); ++i) {
        elem = CoSimIO_ModelPart_GetElementByIndex(model_part, i);
        COSIMIO_CHECK_INT_EQUAL(CoSimIO_Element_Id(elem), ids[i]);
    }

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
