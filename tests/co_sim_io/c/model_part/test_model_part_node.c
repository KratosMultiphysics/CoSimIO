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
#include "c/co_sim_io_c.h"

#include "../checks.h"

int main()
{
    CoSimIO_ModelPart model_part = CoSimIO_CreateModelPart("my_model_part");

    int node_1_id = 3;
    int node_2_id = 8;
    int node_3_id = 16;
    double coords_1[] = {1.0, -2.7, 9.44};
    double coords_2[] = {-1.0, -2.8, 0.0};
    double coords_3[] = {0.0, 3.7, 39.44};

    // the memory of the node is managed on the C++ side
    CoSimIO_Node node_1 = CoSimIO_ModelPart_CreateNewNode(
        model_part,
        node_1_id,
        coords_1[0],
        coords_1[1],
        coords_1[2]);

    CoSimIO_Node node_2 = CoSimIO_ModelPart_CreateNewNode(
        model_part,
        node_2_id,
        coords_2[0],
        coords_2[1],
        coords_2[2]);

    CoSimIO_Node node_3 = CoSimIO_ModelPart_CreateNewNode(
        model_part,
        node_3_id,
        coords_3[0],
        coords_3[1],
        coords_3[2]);

    // checking node_1
    COSIMIO_CHECK_INT_EQUAL(node_1_id, CoSimIO_Node_Id(node_1));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[0], CoSimIO_Node_X(node_1));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[1], CoSimIO_Node_Y(node_1));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[2], CoSimIO_Node_Z(node_1));

    CoSimIO_Node node_1_by_index = CoSimIO_ModelPart_GetNodeByIndex(model_part, 0);

    COSIMIO_CHECK_INT_EQUAL(node_1_id, CoSimIO_Node_Id(node_1_by_index));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[0], CoSimIO_Node_X(node_1_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[1], CoSimIO_Node_Y(node_1_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[2], CoSimIO_Node_Z(node_1_by_index));

    for (int i=0; i<3; ++i) {
        COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[i], CoSimIO_Node_Coordinate(node_1_by_index, i));
    }

    CoSimIO_Node node_1_by_id = CoSimIO_ModelPart_GetNodeById(model_part, node_1_id);

    COSIMIO_CHECK_INT_EQUAL(node_1_id, CoSimIO_Node_Id(node_1_by_id));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[0], CoSimIO_Node_X(node_1_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[1], CoSimIO_Node_Y(node_1_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_1[2], CoSimIO_Node_Z(node_1_by_id));


    // checking node_2
    COSIMIO_CHECK_INT_EQUAL(node_2_id, CoSimIO_Node_Id(node_2));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[0], CoSimIO_Node_X(node_2));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[1], CoSimIO_Node_Y(node_2));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[2], CoSimIO_Node_Z(node_2));

    CoSimIO_Node node_2_by_index = CoSimIO_ModelPart_GetNodeByIndex(model_part, 1);

    COSIMIO_CHECK_INT_EQUAL(node_2_id, CoSimIO_Node_Id(node_2_by_index));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[0], CoSimIO_Node_X(node_2_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[1], CoSimIO_Node_Y(node_2_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[2], CoSimIO_Node_Z(node_2_by_index));

    CoSimIO_Node node_2_by_id = CoSimIO_ModelPart_GetNodeById(model_part, node_2_id);

    COSIMIO_CHECK_INT_EQUAL(node_2_id, CoSimIO_Node_Id(node_2_by_id));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[0], CoSimIO_Node_X(node_2_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[1], CoSimIO_Node_Y(node_2_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_2[2], CoSimIO_Node_Z(node_2_by_id));


    // checking node_3
    COSIMIO_CHECK_INT_EQUAL(node_3_id, CoSimIO_Node_Id(node_3));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[0], CoSimIO_Node_X(node_3));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[1], CoSimIO_Node_Y(node_3));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[2], CoSimIO_Node_Z(node_3));

    CoSimIO_Node node_3_by_index = CoSimIO_ModelPart_GetNodeByIndex(model_part, 2);

    COSIMIO_CHECK_INT_EQUAL(node_3_id, CoSimIO_Node_Id(node_3_by_index));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[0], CoSimIO_Node_X(node_3_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[1], CoSimIO_Node_Y(node_3_by_index));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[2], CoSimIO_Node_Z(node_3_by_index));

    CoSimIO_Node node_3_by_id = CoSimIO_ModelPart_GetNodeById(model_part, node_3_id);

    COSIMIO_CHECK_INT_EQUAL(node_3_id, CoSimIO_Node_Id(node_3_by_id));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[0], CoSimIO_Node_X(node_3_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[1], CoSimIO_Node_Y(node_3_by_id));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords_3[2], CoSimIO_Node_Z(node_3_by_id));

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
