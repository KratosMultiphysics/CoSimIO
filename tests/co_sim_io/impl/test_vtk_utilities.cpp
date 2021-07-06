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

// Project includes
#include "co_sim_io_testing.hpp"
#include "impl/define.hpp"
#include "impl/vtk_utilities.hpp"
#include "impl/filesystem_inc.hpp"


namespace CoSimIO {

TEST_SUITE("VtkUtilities") {

TEST_CASE("write_read_vtk")
{
    ModelPart model_part_write("vtk");
    ModelPart model_part_read("vtk");

    const int node_ids[] = {2, 159, 61};
    const std::array<double, 3> node_coords = {1.0, -2.7, 9.44};
    model_part_write.CreateNewNode(node_ids[0], node_coords[0], node_coords[1], node_coords[2]);
    model_part_write.CreateNewNode(node_ids[1], node_coords[1], node_coords[2], node_coords[0]);
    model_part_write.CreateNewNode(node_ids[2], node_coords[2], node_coords[0], node_coords[1]);

    model_part_write.CreateNewElement(15, CoSimIO::ElementType::Point2D, {node_ids[0]});
    model_part_write.CreateNewElement(73, CoSimIO::ElementType::Line2D2, {node_ids[1], node_ids[2]});
    model_part_write.CreateNewElement(47, CoSimIO::ElementType::Triangle3D3, {node_ids[1], node_ids[2], node_ids[0]});

    const std::string file_name = "vtk_read_write_test.vtk";

    Info info_vtk;
    info_vtk.Set<std::string>("file_name", file_name);

    CoSimIO::Internals::WriteVtk(info_vtk, model_part_write);
    CoSimIO::Internals::ReadVtk(info_vtk, model_part_read);

    fs::remove(file_name);

    CheckModelPartsAreEqual(model_part_write, model_part_read);
}

} // TEST_SUITE("VtkUtilities")

} // namespace CoSimIO
