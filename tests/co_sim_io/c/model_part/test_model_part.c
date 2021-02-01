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

    // COSIMIO_CHECK_STRING_EQUAL(CoSimIO_ModelPart_Name(model_part), "my_model_part");
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 0);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    for (int i=0; i<4; ++i) {
        CoSimIO_ModelPart_CreateNewNode(
            model_part,
            i+1,
            0.1*i,
            i+2,
            -i);
    }

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 4);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 0);

    int connectivity[2];

    for (int i=0; i<2; ++i) {
        connectivity[0] = i+1;
        connectivity[1] = i+2;
        CoSimIO_ModelPart_CreateNewElement(
            model_part,
            i+1,
            CoSimIO_Line2D2,
            connectivity);
    }

    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfNodes(model_part), 4);
    COSIMIO_CHECK_INT_EQUAL(CoSimIO_ModelPart_NumberOfElements(model_part), 2);

    CoSimIO_FreeModelPart(model_part);

    return 0;
}
