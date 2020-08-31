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

TEST_CASE("node_negative_id")
{
    const std::array<double, 3> coords = {1.0, -2.7, 9.44};
    const int id = -16;

    SUBCASE("from_coords")
    {
        CHECK_THROWS_WITH(Node(id, coords[0], coords[1], coords[2]), "Error: "); // TODO find a better way of testing this
    }

    SUBCASE("from_coords_array")
    {
        CHECK_THROWS_WITH(Node(id, coords), "Error: "); // TODO find a better way of testing this
    }
}

TEST_CASE("element_basics")
{
    const int id = 33;
    const std::size_t type = 5;

    Node node(1, 0,0,0);

    Element element(id, type, {&node});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 1);
}

TEST_CASE("element_checks")
{
    const int id = -33;
    const std::size_t type = 5;
    Node node(1, 0,0,0);

    SUBCASE("negative_id")
    {
        CHECK_THROWS_WITH(Element element(id, type, {&node}), "Error: "); // TODO find a better way of testing this
    }

    SUBCASE("no_nodes")
    {
        CHECK_THROWS_WITH(Element element(1, type, {}), "Error: "); // TODO find a better way of testing this
    }
}

TEST_CASE("element_nodes")
{
    const int id = 33;
    const std::size_t type = 5;

    const int node_ids[] = {2, 159, 61};

    const std::array<double, 3> dummy_coords = {0,0,0};
    Node node_1(node_ids[0], dummy_coords);
    Node node_2(node_ids[1], dummy_coords);
    Node node_3(node_ids[2], dummy_coords);

    Element element(id, type, {&node_1, &node_2, &node_3});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 3);

    std::size_t counter=0;
    for (Element::NodesContainerType::const_iterator node_it=element.NodesBegin(); node_it!=element.NodesEnd(); ++node_it) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ((*node_it)->Id(), node_ids[counter]);
        counter++;
    }
}

TEST_CASE("model_part_basics")
{
    ModelPart model_part("for_test");

    CHECK_EQ(model_part.Name(), "for_test");
    CHECK_EQ(model_part.NumberOfNodes(), 0);
    CHECK_EQ(model_part.NumberOfElements(), 0);
}

TEST_CASE("model_part_invalid_names")
{
    CHECK_THROWS_WITH(ModelPart model_part(""), "Error: "); // TODO find a better way of testing this
    CHECK_THROWS_WITH(ModelPart model_part("my_name.ssss"), "Error: "); // TODO find a better way of testing this
}

TEST_CASE("model_part_add_node")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    const auto& new_node = model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 1);

    CHECK_EQ(new_node.Id(), node_id);
    for (std::size_t i=0; i<3; ++i) {
        CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(new_node.Coordinates()[i], doctest::Approx(node_coords[i]));
    }
}

TEST_CASE("model_part_add_nodes")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    const auto& new_node = model_part.CreateNewNode(node_id-1, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 1);

    model_part.CreateNewNode(node_id+1, node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_id+2, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 3);

    CHECK_EQ(new_node.Id(), node_id);
    for (std::size_t i=0; i<3; ++i) {
        CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(new_node.Coordinates()[i], doctest::Approx(node_coords[i]));
    }
}

TEST_CASE("model_part_add_node_twice")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);
    REQUIRE_EQ(model_part.NumberOfNodes(), 1);

    CHECK_THROWS_WITH(model_part.CreateNewNode(1, 0,0,0), "Error: "); // TODO find a better way of testing this
}

TEST_CASE("model_part_get_node")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};

    model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);

    REQUIRE_EQ(model_part.NumberOfNodes(), 1);

    SUBCASE("existing_node")
    {
        const auto& r_node = model_part.GetNode(node_id);

        CHECK_EQ(r_node.Id(), node_id);
        for (std::size_t i=0; i<3; ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CHECK_EQ(r_node.Coordinates()[i], doctest::Approx(node_coords[i]));
        }
    }

    SUBCASE("non_existing_node")
    {
        CHECK_THROWS_WITH(model_part.GetNode(node_id+1), "Error: "); // TODO find a better way of testing this
    }
}

} // TEST_SUITE("ModelPart")

} // namespace CoSimIO
