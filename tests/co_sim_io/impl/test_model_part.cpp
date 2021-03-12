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

// System includes
#include <sstream>

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
        CHECK_THROWS_WITH(Node(id, coords[0], coords[1], coords[2]), "Error: Id must be >= 1!\n");
    }

    SUBCASE("from_coords_array")
    {
        CHECK_THROWS_WITH(Node(id, coords), "Error: Id must be >= 1!\n");
    }
}

TEST_CASE("node_ostream")
{
    Node node(1, 0.2, -33.4, 647);

    std::stringstream test_stream;

    test_stream << node;

    const std::string exp_string = "CoSimIO-Node; Id: 1\n    Coordinates: [ 0.2 | -33.4 | 647 ]\n";

    CHECK_EQ(test_stream.str(), exp_string);
}

TEST_CASE("element_basics")
{
    const int id = 33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;

    Node node(1, 0,0,0);

    Element element(id, type, {&node});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 1);
}

TEST_CASE("element_checks")
{
    const int id = -33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;
    Node node(1, 0,0,0);

    SUBCASE("negative_id")
    {
        CHECK_THROWS_WITH(Element element(id, type, {&node}), "Error: Id must be >= 1!\n");
    }

    SUBCASE("no_nodes")
    {
        CHECK_THROWS_WITH(Element element(1, type, {}), "Error: No nodes were passed!\n");
    }

    SUBCASE("wrong_element_type")
    {
        CHECK_THROWS_WITH(Element element(1, CoSimIO::ElementType::Line2D2, {&node}), "Error: Number of nodes (1) does not match expected number for element type (2)!\n");
    }
}

TEST_CASE("element_nodes")
{
    const int id = 33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Triangle2D3;

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
    for (auto node_it=element.NodesBegin(); node_it!=element.NodesEnd(); ++node_it) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ((*node_it)->Id(), node_ids[counter]);
        counter++;
    }
}

TEST_CASE("element_ostream")
{
    const std::array<double, 3> dummy_coords = {0,0,0};
    Node node_1(1, dummy_coords);
    Node node_2(22, dummy_coords);
    Node node_3(321, dummy_coords);

    Element element(65, CoSimIO::ElementType::Triangle2D3, {&node_1, &node_2, &node_3});

    std::stringstream test_stream;

    test_stream << element;

    const std::string exp_string = "CoSimIO-Element; Id: 65\n    Number of Nodes: 3\n    Node Ids: 1, 22, 321\n";

    CHECK_EQ(test_stream.str(), exp_string);
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
    CHECK_THROWS_WITH(ModelPart model_part(""), "Error: Please don't use empty names (\"\") when creating a ModelPart\n");
    CHECK_THROWS_WITH(ModelPart model_part("my_name.ssss"), "Error: Please don't use names containing (\".\") when creating a ModelPart (used in \"my_name.ssss\")\n");
}

TEST_CASE("model_part_create_new_node")
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

TEST_CASE("model_part_create_new_nodes")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    const auto& new_node = model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 1);

    model_part.CreateNewNode(node_id+1, node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_id+2, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 3);

    CHECK_EQ(new_node.Id(), node_id);
    for (std::size_t i=0; i<3; ++i) {
        CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(new_node.Coordinates()[i], doctest::Approx(node_coords[i]));
    }

    std::size_t counter=0;
    for (auto node_it=model_part.NodesBegin(); node_it!=model_part.NodesEnd(); ++node_it) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ((*node_it)->Id(), node_id+counter);
        counter++;
    }
}

TEST_CASE("model_part_create_new_node_twice")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);
    REQUIRE_EQ(model_part.NumberOfNodes(), 1);

    CHECK_THROWS_WITH(model_part.CreateNewNode(1, 0,0,0), "Error: The Node with Id 1 exists already!\n");
}

TEST_CASE("model_part_get_node")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};

    model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);

    REQUIRE_EQ(model_part.NumberOfNodes(), 1);

    SUBCASE("existing")
    {
        const auto& r_node = model_part.GetNode(node_id);

        CHECK_EQ(r_node.Id(), node_id);
        for (std::size_t i=0; i<3; ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CHECK_EQ(r_node.Coordinates()[i], doctest::Approx(node_coords[i]));
        }
    }

    SUBCASE("existing_pointer_version")
    {
        const auto p_node = model_part.pGetNode(node_id);

        CHECK_EQ(p_node->Id(), node_id);
        for (std::size_t i=0; i<3; ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CHECK_EQ(p_node->Coordinates()[i], doctest::Approx(node_coords[i]));
        }
    }

    SUBCASE("non_existing")
    {
        CHECK_THROWS_WITH(model_part.GetNode(node_id+1), "Error: Node with Id 692 does not exist!\n");
    }
}

TEST_CASE("model_part_create_new_element")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 1);

    const int elem_id = 47;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;

    const auto& new_elem = model_part.CreateNewElement(elem_id, type, {node_id});

    CHECK_EQ(new_elem.Id(), elem_id);
    CHECK_EQ(new_elem.Type(), type);
    REQUIRE_EQ(new_elem.NumberOfNodes(), 1);

    for (auto node_it=new_elem.NodesBegin(); node_it!=new_elem.NodesEnd(); ++node_it) {
        CHECK_EQ((*node_it)->Id(), node_id);
        for (std::size_t i=0; i<3; ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CHECK_EQ((*node_it)->Coordinates()[i], doctest::Approx(node_coords[i]));
        }
    }
}

TEST_CASE("model_part_create_new_elements")
{
    ModelPart model_part("for_test");

    const int node_ids[] = {2, 159, 61};
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);

    CHECK_EQ(model_part.NumberOfNodes(), 3);

    const int elem_ids[] = {21, 19, 961};
    const std::size_t elem_num_nodes[] = {1,1,2};
    const CoSimIO::ElementType elem_types[] = {
        CoSimIO::ElementType::Point2D,
        CoSimIO::ElementType::Point2D,
        CoSimIO::ElementType::Line2D2
    };

    model_part.CreateNewElement(elem_ids[0], elem_types[0], {node_ids[0]});
    model_part.CreateNewElement(elem_ids[1], elem_types[1], {node_ids[1]});
    model_part.CreateNewElement(elem_ids[2], elem_types[2], {node_ids[1], node_ids[2]});

    REQUIRE_EQ(model_part.NumberOfElements(), 3);

    std::size_t counter = 0;
    for (auto elem_it=model_part.ElementsBegin(); elem_it!=model_part.ElementsEnd(); ++elem_it) {
        CHECK_EQ((*elem_it)->Id(), elem_ids[counter]);
        CHECK_EQ((*elem_it)->Type(), elem_types[counter]);
        REQUIRE_EQ((*elem_it)->NumberOfNodes(), elem_num_nodes[counter]);
        counter++;
    }
}

TEST_CASE("model_part_create_new_element_twice")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);
    model_part.CreateNewElement(1, CoSimIO::ElementType::Point2D, {1});
    REQUIRE_EQ(model_part.NumberOfElements(), 1);

    CHECK_THROWS_WITH(model_part.CreateNewElement(1, CoSimIO::ElementType::Point2D, {1}), "Error: The Element with Id 1 exists already!\n");
}

TEST_CASE("model_part_get_element")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);

    const int elem_id = 6;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;
    model_part.CreateNewElement(elem_id, type, {1});
    REQUIRE_EQ(model_part.NumberOfElements(), 1);

    SUBCASE("existing")
    {
        const auto& r_elem = model_part.GetElement(elem_id);

        CHECK_EQ(r_elem.Id(), elem_id);
        CHECK_EQ(r_elem.Type(), type);
        CHECK_EQ(r_elem.NumberOfNodes(), 1);
    }

    SUBCASE("existing_pointer_version")
    {
        const auto p_elem = model_part.pGetElement(elem_id);

        CHECK_EQ(p_elem->Id(), elem_id);
        CHECK_EQ(p_elem->Type(), type);
        CHECK_EQ(p_elem->NumberOfNodes(), 1);
    }

    SUBCASE("non_existing")
    {
        CHECK_THROWS_WITH(model_part.GetElement(elem_id+1), "Error: Element with Id 7 does not exist!\n");
    }
}

TEST_CASE("model_part_clear")
{
    ModelPart model_part("for_test");

    const int node_ids[] = {2, 159, 61};
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);

    model_part.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});
    model_part.CreateNewElement(73, CoSimIO::ElementType::Line2D2, {node_ids[1], node_ids[2]});
    model_part.CreateNewElement(47, CoSimIO::ElementType::Triangle3D3, {node_ids[1], node_ids[2], node_ids[0]});
    model_part.CreateNewElement(18, CoSimIO::ElementType::Point3D, {node_ids[1]});

    REQUIRE_EQ(model_part.NumberOfNodes(), 3);
    REQUIRE_EQ(model_part.NumberOfElements(), 4);

    // remove all Nodes and Elements
    model_part.Clear();

    REQUIRE_EQ(model_part.NumberOfNodes(), 0);
    REQUIRE_EQ(model_part.NumberOfElements(), 0);
}

TEST_CASE("model_part_ostream")
{
    ModelPart model_part("for_test");

    std::string exp_string;

    SUBCASE("empty")
    {
        exp_string = "CoSimIO-ModelPart \"for_test\"\n    Number of Nodes: 0\n    Number of Elements: 0\n";
    }

    SUBCASE("with_entities")
    {
        const int node_ids[] = {2, 159, 61};
        const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
        model_part.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
        model_part.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
        model_part.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);

        model_part.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});

        exp_string = "CoSimIO-ModelPart \"for_test\"\n    Number of Nodes: 3\n    Number of Elements: 1\n";
    }

    std::stringstream test_stream;

    test_stream << model_part;

    CHECK_EQ(test_stream.str(), exp_string);
}

} // TEST_SUITE("ModelPart")

} // namespace CoSimIO
