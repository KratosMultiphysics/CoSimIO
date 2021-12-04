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
#include "includes/model_part.hpp"
#include "includes/stream_serializer.hpp"


namespace CoSimIO {

namespace {

template<class TContainerType>
bool PointerVectorChecker(const TContainerType& rContainer)
{
    const auto it_begin = rContainer.begin();

    for (std::size_t i=0; i<rContainer.size(); ++i) {
        const auto it = it_begin + i;
        if (i+1 != static_cast<std::size_t>((*it).Id())) return false;
    }

    return true;
}

}

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

TEST_CASE("node_serialization")
{
    Node node_save(85, 0.2, -33.4, 647);
    Node node_load(1, 0,0,0);

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("node", node_save);
    serializer.load("node", node_load);

    CheckNodesAreEqual(node_save, node_load);
}

TEST_CASE("element_basics")
{
    const int id = 33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;

    auto p_node = CoSimIO::make_intrusive<CoSimIO::Node>(1, 0,0,0);

    Element element(id, type, {p_node});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 1);
}

TEST_CASE("element_checks")
{
    const int id = -33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Point2D;
    auto p_node = CoSimIO::make_intrusive<CoSimIO::Node>(1, 0,0,0);

    SUBCASE("negative_id")
    {
        CHECK_THROWS_WITH(Element element(id, type, {p_node}), "Error: Id must be >= 1!\n");
    }

    SUBCASE("no_nodes")
    {
        CHECK_THROWS_WITH(Element element(1, type, {}), "Error: No nodes were passed!\n");
    }

    SUBCASE("wrong_element_type")
    {
        CHECK_THROWS_WITH(Element element(1, CoSimIO::ElementType::Line2D2, {p_node}), "Error: Number of nodes (1) does not match expected number for element type (2)!\n");
    }
}

TEST_CASE("element_nodes")
{
    const int id = 33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Triangle2D3;

    const int node_ids[] = {2, 159, 61};

    const std::array<double, 3> dummy_coords = {0,0,0};
    auto p_node_1 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[0], dummy_coords);
    auto p_node_2 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[1], dummy_coords);
    auto p_node_3 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[2], dummy_coords);

    Element element(id, type, {p_node_1, p_node_2, p_node_3});

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

TEST_CASE("element_range_based_loop_nodes")
{
    const int id = 33;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Triangle2D3;

    const int node_ids[] = {2, 159, 61};

    const std::array<double, 3> dummy_coords = {0,0,0};
    auto p_node_1 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[0], dummy_coords);
    auto p_node_2 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[1], dummy_coords);
    auto p_node_3 = CoSimIO::make_intrusive<CoSimIO::Node>(node_ids[2], dummy_coords);

    Element element(id, type, {p_node_1, p_node_2, p_node_3});

    CHECK_EQ(element.Id(), id);
    CHECK_EQ(element.Type(), type);
    CHECK_EQ(element.NumberOfNodes(), 3);

    std::size_t counter=0;
    for (const auto& r_node : element.Nodes()) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(r_node.Id(), node_ids[counter]);
        counter++;
    }
}

TEST_CASE("element_ostream")
{
    const std::array<double, 3> dummy_coords = {0,0,0};
    auto p_node_1 = CoSimIO::make_intrusive<CoSimIO::Node>(1, dummy_coords);
    auto p_node_2 = CoSimIO::make_intrusive<CoSimIO::Node>(22, dummy_coords);
    auto p_node_3 = CoSimIO::make_intrusive<CoSimIO::Node>(321, dummy_coords);

    Element element(65, CoSimIO::ElementType::Triangle2D3, {p_node_1, p_node_2, p_node_3});

    std::stringstream test_stream;

    test_stream << element;

    const std::string exp_string = "CoSimIO-Element; Id: 65\n    Number of Nodes: 3\n    Node Ids: 1, 22, 321\n";

    CHECK_EQ(test_stream.str(), exp_string);
}

TEST_CASE("element_serialization")
{
    const std::array<double, 3> dummy_coords = {1.6, -2.2, 17};
    auto p_node_1 = CoSimIO::make_intrusive<CoSimIO::Node>(1, dummy_coords[0], dummy_coords[1], dummy_coords[2]);
    auto p_node_2 = CoSimIO::make_intrusive<CoSimIO::Node>(22, dummy_coords[1], dummy_coords[2], dummy_coords[0]);
    auto p_node_3 = CoSimIO::make_intrusive<CoSimIO::Node>(321, dummy_coords[2], dummy_coords[0], dummy_coords[1]);
    auto p_dummy_node = CoSimIO::make_intrusive<CoSimIO::Node>(1111, 0,0,0);

    Element element_save(65, CoSimIO::ElementType::Triangle2D3, {p_node_1, p_node_2, p_node_3});
    Element element_load(1,  CoSimIO::ElementType::Point2D, {p_dummy_node});

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("element", element_save);
    serializer.load("element", element_load);

    CheckElementsAreEqual(element_save, element_load);
}

TEST_CASE("model_part_basics")
{
    ModelPart model_part("for_test");

    CHECK_EQ(model_part.Name(), "for_test");
    CHECK_EQ(model_part.NumberOfNodes(), 0);
    CHECK_EQ(model_part.NumberOfLocalNodes(), 0);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);
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
    CHECK_EQ(model_part.NumberOfLocalNodes(), 1);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);

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
    CHECK_EQ(model_part.NumberOfLocalNodes(), 1);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);

    model_part.CreateNewNode(node_id+1, node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_id+2, node_coords[0], node_coords[1], node_coords[2]);
    CHECK_EQ(model_part.NumberOfNodes(), 3);
    CHECK_EQ(model_part.NumberOfLocalNodes(), 3);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);

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

TEST_CASE("model_part_range_based_loop_nodes")
{
    ModelPart model_part("for_test");

    for (std::size_t i=0; i<5; ++i) {
        model_part.CreateNewNode(i+1, 0,0,0);
    }

    REQUIRE_EQ(model_part.NumberOfNodes(), 5);

    std::size_t counter = 0;
    for (const auto& r_node : model_part.Nodes()) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(r_node.Id(), counter+1);
        counter++;
    }
}

TEST_CASE("model_part_ghost_nodes")
{
    ModelPart model_part("for_test");

    for (std::size_t i=0; i<5; ++i) {
        model_part.CreateNewNode(i+1, 0,0,0);
    }

    const int partition_index = 63;
    const int partition_index_2 = 18;

    CHECK_EQ(model_part.NumberOfNodes(), 5);
    CHECK_EQ(model_part.NumberOfLocalNodes(), 5);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);

    SUBCASE("already_existing_regular_node")
    {
        CHECK_THROWS_WITH(model_part.CreateNewGhostNode(2, 0,0,0, partition_index), "Error: The Node with Id 2 exists already!\n");
    }

    SUBCASE("create_ghost_nodes")
    {
        for (std::size_t i=5; i<8; ++i) {
            model_part.CreateNewGhostNode(i+1, 0,0,0, partition_index);
        }
        CHECK_EQ(model_part.NumberOfNodes(), 8);
        CHECK_EQ(model_part.NumberOfLocalNodes(), 5);
        CHECK_EQ(model_part.NumberOfGhostNodes(), 3);

        CHECK_EQ(model_part.GetPartitionModelParts().count(partition_index), 1);
        CHECK_EQ(model_part.GetPartitionModelParts().size(), 1);

        model_part.CreateNewGhostNode(118, 0,0,0, partition_index_2);

        CHECK_EQ(model_part.NumberOfNodes(), 9);
        CHECK_EQ(model_part.NumberOfLocalNodes(), 5);
        CHECK_EQ(model_part.NumberOfGhostNodes(), 4);
        CHECK_EQ(model_part.GetPartitionModelParts().count(partition_index), 1);
        CHECK_EQ(model_part.GetPartitionModelParts().count(partition_index_2), 1);
        CHECK_EQ(model_part.GetPartitionModelParts().size(), 2);
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

TEST_CASE("model_part_create_new_element_with_ghost_node")
{
    ModelPart model_part("for_test");

    const int node_id = 691;
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part.CreateNewNode(node_id, node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewGhostNode(node_id+1, node_coords[0], node_coords[1], node_coords[2], 15);
    REQUIRE_EQ(model_part.NumberOfNodes(), 2);

    const int elem_id = 47;
    const CoSimIO::ElementType type = CoSimIO::ElementType::Line2D2;

    const auto& new_elem = model_part.CreateNewElement(elem_id, type, {node_id, node_id+1});

    CHECK_EQ(new_elem.Id(), elem_id);
    CHECK_EQ(new_elem.Type(), type);
    REQUIRE_EQ(new_elem.NumberOfNodes(), 2);
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

TEST_CASE("model_part_range_based_loop_elements")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);

    for (std::size_t i=0; i<4; ++i) {
        model_part.CreateNewElement(i+1, CoSimIO::ElementType::Point2D, {1});
    }

    REQUIRE_EQ(model_part.NumberOfElements(), 4);

    std::size_t counter = 0;
    for (const auto& r_elem : model_part.Elements()) {
        CAPTURE(counter); // log the current input data (done manually as not fully supported yet by doctest)
        CHECK_EQ(r_elem.Id(), counter+1);
        counter++;
    }
}

TEST_CASE("model_part_clear")
{
    ModelPart model_part("for_test");
    const ModelPart& r_const_ref = model_part;

    const int node_ids[] = {2, 159, 61, 19, 874};
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);
    model_part.CreateNewGhostNode(node_ids[3], node_coords[2], node_coords[0], node_coords[1], 125);
    model_part.CreateNewGhostNode(node_ids[4], node_coords[2], node_coords[0], node_coords[1], 987);

    model_part.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});
    model_part.CreateNewElement(73, CoSimIO::ElementType::Line2D2, {node_ids[1], node_ids[2]});
    model_part.CreateNewElement(47, CoSimIO::ElementType::Triangle3D3, {node_ids[1], node_ids[2], node_ids[0]});
    model_part.CreateNewElement(18, CoSimIO::ElementType::Point3D, {node_ids[1]});

    CHECK_EQ(model_part.NumberOfNodes(), 5);
    CHECK_EQ(model_part.NumberOfElements(), 4);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfNodes(), 3);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfNodes(), 2);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfElements(), 4);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfElements(), 0);

    // remove all Nodes and Elements
    model_part.Clear();

    CHECK_EQ(model_part.NumberOfNodes(), 0);
    CHECK_EQ(model_part.NumberOfLocalNodes(), 0);
    CHECK_EQ(model_part.NumberOfGhostNodes(), 0);
    CHECK_EQ(model_part.NumberOfElements(), 0);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfNodes(), 0);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfNodes(), 0);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfElements(), 0);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfElements(), 0);
    CHECK_EQ(model_part.GetPartitionModelParts().size(), 0);

    model_part.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);
    model_part.CreateNewGhostNode(node_ids[3], node_coords[2], node_coords[0], node_coords[1], 125);
    model_part.CreateNewGhostNode(node_ids[4], node_coords[2], node_coords[0], node_coords[1], 987);

    model_part.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});
    model_part.CreateNewElement(73, CoSimIO::ElementType::Line2D2, {node_ids[1], node_ids[2]});
    model_part.CreateNewElement(47, CoSimIO::ElementType::Triangle3D3, {node_ids[1], node_ids[2], node_ids[0]});
    model_part.CreateNewElement(18, CoSimIO::ElementType::Point3D, {node_ids[1]});

    CHECK_EQ(model_part.NumberOfNodes(), 5);
    CHECK_EQ(model_part.NumberOfElements(), 4);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfNodes(), 3);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfNodes(), 2);
    CHECK_EQ(r_const_ref.GetLocalModelPart().NumberOfElements(), 4);
    CHECK_EQ(r_const_ref.GetGhostModelPart().NumberOfElements(), 0);
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
        const std::vector<IdType> node_ids {2, 159, 61};
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

TEST_CASE("model_part_serialization")
{
    ModelPart model_part_save("for_test");
    ModelPart model_part_load("xxx");

    const int node_ids[] = {2, 159, 61, 1996};
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part_save.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part_save.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part_save.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);
    model_part_save.CreateNewGhostNode(node_ids[3], node_coords[2], node_coords[0], node_coords[1], 15);

    model_part_save.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});
    model_part_save.CreateNewElement(73, CoSimIO::ElementType::Line2D2, {node_ids[1], node_ids[2]});
    model_part_save.CreateNewElement(47, CoSimIO::ElementType::Triangle3D3, {node_ids[1], node_ids[2], node_ids[0]});

    CoSimIO::Internals::StreamSerializer serializer;
    serializer.save("model_part", model_part_save);
    serializer.load("model_part", model_part_load);

    CheckModelPartsAreEqual(model_part_save, model_part_load);
}


TEST_CASE("model_part_pointer_vector")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);
    model_part.CreateNewNode(2, 0,0,0);
    model_part.CreateNewNode(3, 0,0,0);
    model_part.CreateNewNode(4, 0,0,0);

    model_part.CreateNewElement(1, CoSimIO::ElementType::Point2D, {1});
    model_part.CreateNewElement(2, CoSimIO::ElementType::Point2D, {2});
    model_part.CreateNewElement(3, CoSimIO::ElementType::Point2D, {3});

    CHECK_EQ(model_part.NumberOfNodes(), 4);
    CHECK_EQ(model_part.NumberOfElements(), 3);

    CHECK_EQ(model_part.Nodes().size(), 4);
    CHECK_EQ(model_part.Elements().size(), 3);

    CHECK(PointerVectorChecker(model_part.Nodes()));
    CHECK(PointerVectorChecker(model_part.Elements()));
}

} // TEST_SUITE("ModelPart")

} // namespace CoSimIO
