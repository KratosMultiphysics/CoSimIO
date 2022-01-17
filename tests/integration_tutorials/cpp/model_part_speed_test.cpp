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
#include <numeric>

// CoSimulation includes
#include "co_sim_io.hpp"
#include "/home/philipp/software/CoSimIO/co_sim_io/includes/utilities.hpp"

int main()
{
    std::size_t num_nodes = 1e3;

    while(true) {
        CoSimIO::ModelPart model_part("for_test");
        const auto start_time(std::chrono::steady_clock::now());
        for (std::size_t i=0; i<num_nodes; ++i) {
            model_part.CreateNewNode(i+1, 0,0,0);
        }
        std::cout << "Creating " << num_nodes << " nodes one afer the other took " << CoSimIO::Utilities::ElapsedSeconds(start_time) << " [s]" << std::endl;

        // CoSimIO::ModelPart model_part_2("for_test");
        // std::vector<CoSimIO::IdType> ids(num_nodes);
        // std::vector<double> xx(num_nodes, 0.0);
        // std::vector<double> yy(num_nodes, 0.0);
        // std::vector<double> zz(num_nodes, 0.0);

        // std::iota(ids.begin(), ids.end(), 1);

        // const auto start_time_2(std::chrono::steady_clock::now());
        // model_part_2.CreateNewNodes(ids, xx, yy, zz);
        // std::cout << "Creating " << num_nodes << " nodes all at once took        " << CoSimIO::Utilities::ElapsedSeconds(start_time_2) << " [s]\n" << std::endl;

        num_nodes *= 2;

        if (CoSimIO::Utilities::ElapsedSeconds(start_time) > 10) break;
    }

    // std::size_t num_nodes_get = 1e3;
    // while(true) {
    //     CoSimIO::ModelPart model_part("for_test");
    //     for (std::size_t i=0; i<num_nodes_get; ++i) {
    //         model_part.CreateNewNode(i+1, 0,0,0);
    //     }

    //     const auto start_time(std::chrono::steady_clock::now());

    //     std::size_t ids{0};
    //     for (std::size_t i=0; i<num_nodes_get; ++i) {
    //         const auto& node = model_part.GetNode(i+1);
    //         ids += node.Id();
    //     }
    //     std::cout << "Getting " << num_nodes_get << " nodes took " << CoSimIO::Utilities::ElapsedSeconds(start_time) << " [s]" << std::endl;
    //     num_nodes_get *= 2;

    //     if (CoSimIO::Utilities::ElapsedSeconds(start_time) > 10) break;
    // }
    return 0;
}
