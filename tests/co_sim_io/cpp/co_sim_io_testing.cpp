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
#include "co_sim_io_testing.hpp"

void CheckNodesAreEqual(
    const CoSimIO::Node& Node1,
    const CoSimIO::Node& Node2)
{
    CHECK_EQ(Node1.Id(), Node2.Id());
    CHECK_EQ(Node1.X(), doctest::Approx(Node2.X()));
    CHECK_EQ(Node1.Y(), doctest::Approx(Node2.Y()));
    CHECK_EQ(Node1.Z(), doctest::Approx(Node2.Z()));
}

void CheckElementsAreEqual(
    const CoSimIO::Element& Element1,
    const CoSimIO::Element& Element2)
{
    // basic checks
    CHECK_EQ(Element1.Id(), Element2.Id());
    CHECK_EQ(Element1.Type(), Element2.Type());
    REQUIRE_EQ(Element1.NumberOfNodes(), Element2.NumberOfNodes());

    // check nodes
    for (std::size_t i=0; i< Element1.NumberOfNodes(); ++i) {
        CheckNodesAreEqual(**(Element1.NodesBegin()+i), **(Element2.NodesBegin()+i));
    }
}

void CheckModelPartsAreEqual(
    const CoSimIO::ModelPart& ModelPart1,
    const CoSimIO::ModelPart& ModelPart2)
{
    // basic checks
    CHECK_EQ(ModelPart1.Name(), ModelPart2.Name());
    REQUIRE_EQ(ModelPart1.NumberOfNodes(), ModelPart2.NumberOfNodes());
    REQUIRE_EQ(ModelPart1.NumberOfElements(), ModelPart2.NumberOfElements());

    // check nodes
    for (std::size_t i=0; i< ModelPart1.NumberOfNodes(); ++i) {
        CheckNodesAreEqual(**(ModelPart1.NodesBegin()+i), **(ModelPart2.NodesBegin()+i));
    }

    // check elements
    for (std::size_t i=0; i< ModelPart1.NumberOfElements(); ++i) {
        CheckElementsAreEqual(**(ModelPart1.ElementsBegin()+i), **(ModelPart2.ElementsBegin()+i));
    }
}
