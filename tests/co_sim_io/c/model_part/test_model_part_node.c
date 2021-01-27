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

    int node_id = 3;
    double coords[] = {1.0, -2.7, 9.44};

    // the memory of the node is managed on the C++ side
    CoSimIO_Node node = CoSimIO_ModelPart_CreateNewNode(
        model_part,
        node_id,
        coords[0],
        coords[1],
        coords[2]);

    COSIMIO_CHECK_INT_EQUAL(node_id, CoSimIO_Node_Id(node));

    COSIMIO_CHECK_DOUBLE_EQUAL(coords[0], CoSimIO_Node_X(node));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords[1], CoSimIO_Node_Y(node));
    COSIMIO_CHECK_DOUBLE_EQUAL(coords[2], CoSimIO_Node_Z(node));

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
