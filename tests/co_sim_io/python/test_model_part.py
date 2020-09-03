#     ______     _____ _           ________
#    / ____/___ / ___/(_)___ ___  /  _/ __ |
#   / /   / __ \\__ \/ / __ `__ \ / // / / /
#  / /___/ /_/ /__/ / / / / / / // // /_/ /
#  \____/\____/____/_/_/ /_/ /_/___/\____/
#  Kratos CoSimulationApplication
#
#  License:         BSD License, see license.txt
#
#  Main authors:    Philipp Bucher (https://github.com/philbucher)
#

# tests for the python exposure of CoSimIO::Info

# python imports
import unittest

import CoSimIO


'''
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

TEST_CASE("element_ostream")
{
    const std::array<double, 3> dummy_coords = {0,0,0};
    Node node_1(1, dummy_coords);
    Node node_2(22, dummy_coords);
    Node node_3(321, dummy_coords);

    Element element(65, 5, {&node_1, &node_2, &node_3});

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
    CHECK_THROWS_WITH(ModelPart model_part(""), "Error: "); // TODO find a better way of testing this
    CHECK_THROWS_WITH(ModelPart model_part("my_name.ssss"), "Error: "); // TODO find a better way of testing this
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
}

TEST_CASE("model_part_create_new_node_twice")
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

    SUBCASE("existing")
    {
        const auto& r_node = model_part.GetNode(node_id);

        CHECK_EQ(r_node.Id(), node_id);
        for (std::size_t i=0; i<3; ++i) {
            CAPTURE(i); // log the current input data (done manually as not fully supported yet by doctest)
            CHECK_EQ(r_node.Coordinates()[i], doctest::Approx(node_coords[i]));
        }
    }

    SUBCASE("non_existing")
    {
        CHECK_THROWS_WITH(model_part.GetNode(node_id+1), "Error: "); // TODO find a better way of testing this
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
    const std::size_t type = 5;

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
    const std::size_t elem_types[] = {5, 5, 9};
    const std::size_t elem_num_nodes[] = {1,1,2};

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
    model_part.CreateNewElement(1, 5, {1});
    REQUIRE_EQ(model_part.NumberOfElements(), 1);

    CHECK_THROWS_WITH(model_part.CreateNewElement(1, 5, {1}), "Error: "); // TODO find a better way of testing this
}

TEST_CASE("model_part_get_element")
{
    ModelPart model_part("for_test");

    model_part.CreateNewNode(1, 0,0,0);

    const int elem_id = 6;
    model_part.CreateNewElement(elem_id, 5, {1});
    REQUIRE_EQ(model_part.NumberOfElements(), 1);

    SUBCASE("existing")
    {
        const auto& r_elem = model_part.GetElement(elem_id);

        CHECK_EQ(r_elem.Id(), elem_id);
        CHECK_EQ(r_elem.Type(), 5);
        CHECK_EQ(r_elem.NumberOfNodes(), 1);
    }

    SUBCASE("non_existing")
    {
        CHECK_THROWS_WITH(model_part.GetElement(elem_id+1), "Error: "); // TODO find a better way of testing this
    }
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

        model_part.CreateNewElement(15, 1, {node_ids[0]});

        exp_string = "CoSimIO-ModelPart \"for_test\"\n    Number of Nodes: 3\n    Number of Elements: 1\n";
    }

    std::stringstream test_stream;

    test_stream << model_part;

    CHECK_EQ(test_stream.str(), exp_string);
}
'''

class CoSimIO_ModelPart(unittest.TestCase):
    maxDiff = None # to display all the diff

    def test_node(self):
        coords = [1.0, -2.7, 9.44]
        node_id = 16

        with self.subTest("from_coords"):
            node = CoSimIO.Node(node_id, coords[0], coords[1], coords[2])

        with self.subTest("from_coords_array"):
            node = CoSimIO.Node(node_id, coords)

        self.assertEqual(node.Id(), node_id)
        self.assertAlmostEqual(node.X(), coords[0])
        self.assertAlmostEqual(node.Y(), coords[1])
        self.assertAlmostEqual(node.Z(), coords[2])

        for i in range(3):
            self.assertAlmostEqual(coords[i], node.Coordinates()[i], msg=str(i))

    def test_print_node(self):
        coords = [1.0, -2.7, 9.44]
        node_id = 16

        node = CoSimIO.Node(node_id, coords)

        exp_string = "CoSimIO-Node; Id: 16\n    Coordinates: [ 1 | -2.7 | 9.44 ]\n"
        self.assertMultiLineEqual(str(node), exp_string)



if __name__ == '__main__':
    unittest.main()
