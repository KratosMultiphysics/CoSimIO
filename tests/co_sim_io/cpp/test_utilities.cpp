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
#include "includes/define.hpp"
#include "includes/utilities.hpp"


namespace CoSimIO {
namespace Internals {

TEST_SUITE("Utilities") {

TEST_CASE("ComputeNeighborRanks")
{
    std::unordered_set<std::size_t> exp_neighbor_ranks;
    std::unordered_set<std::size_t> neighbor_ranks;

    SUBCASE("serial")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 1;
        exp_neighbor_ranks = {0};
        neighbor_ranks = ComputeNeighborRanks(
            my_rank,
            my_size,
            partner_size);
    }

    SUBCASE("partner_distributed")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 5;
        exp_neighbor_ranks = {0,1,2,3,4};

        std::cout << exp_neighbor_ranks << std::endl;

        neighbor_ranks = ComputeNeighborRanks(
            my_rank,
            my_size,
            partner_size);
    }


    CHECK_EQ(exp_neighbor_ranks, neighbor_ranks);


// std::unordered_set<std::size_t> ComputeNeighborRanks(
//     const std::size_t MyRank,
//     const std::size_t MySize,
//     const std::size_t PartnerSize);
}

} // TEST_SUITE("VtkUtilities")

} // namespace Internals
} // namespace CoSimIO
