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
#include "impl/model_part.hpp"


namespace CoSimIO {

TEST_SUITE("ModelPart") {

TEST_CASE("node")
{
    const std::array<double, 3> coords = {1.0, -2.7, 9.44};
    const int id = 16;

    std::unique_ptr<Node> p_node;

    SUBCASE("from_coords")
    {
        p_node = CoSimIO::make_unique<Node>(id, coords[0], coords[1], coords[2]);
    }

    SUBCASE("from_coords_array")
    {
        p_node = CoSimIO::make_unique<Node>(id, coords);
    }

    CHECK_EQ(p_node->Id(), id);

    CHECK_EQ(p_node->X(), doctest::Approx(coords[0]));
    CHECK_EQ(p_node->Y(), doctest::Approx(coords[1]));
    CHECK_EQ(p_node->Z(), doctest::Approx(coords[2]));

    for (std::size_t i=0; i<3; ++i) {
        CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(p_node->Coordinates()[i], doctest::Approx(coords[i]));
    }
}

TEST_CASE("element_basics")
{
    const int id = 33;
    const std::size_t type = 5;
    Element element(id, type, {});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 0);
}

} // TEST_SUITE("ModelPart")

} // namespace CoSimIO
