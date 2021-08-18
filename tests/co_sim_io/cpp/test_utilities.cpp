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

TEST_SUITE("Utilities") {

TEST_CASE("ComputePartnerRanksAsImporter")
{
    std::set<std::size_t> exp_partner_ranks;
    std::set<std::size_t> neighbor_ranks;

    SUBCASE("serial_serial")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 1;
        exp_partner_ranks = {0};
        neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
            my_rank,
            my_size,
            partner_size);
    }

    SUBCASE("serial_distributed")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 5;
        exp_partner_ranks = {0,1,2,3,4};

        neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
            my_rank,
            my_size,
            partner_size);
    }

    SUBCASE("distributed_serial")
    {
        const std::size_t my_size = 4;
        const std::size_t partner_size = 1;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_same_size")
    {
        const std::size_t my_size = 2;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_1")
    {
        const std::size_t my_size = 4;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_2")
    {
        const std::size_t my_size = 7;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=4")
        {
            const std::size_t my_rank = 4;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=5")
        {
            const std::size_t my_rank = 5;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=6")
        {
            const std::size_t my_rank = 6;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_3")
    {
        const std::size_t my_size = 5;
        const std::size_t partner_size = 3;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=4")
        {
            const std::size_t my_rank = 4;
            exp_partner_ranks = {};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_1")
    {
        const std::size_t my_size = 2;
        const std::size_t partner_size = 4;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0,1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {2,3};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_2")
    {
        const std::size_t my_size = 3;
        const std::size_t partner_size = 7;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0,1,2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {3,4,5};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {6};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_3")
    {
        const std::size_t my_size = 3;
        const std::size_t partner_size = 15;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0,1,2,3,4};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {5,6,7,8,9};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {10,11,12,13,14};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_4")
    {
        const std::size_t my_size = 3;
        const std::size_t partner_size = 8;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0,1,2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {3,4,5};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {6,7};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_5")
    {
        const std::size_t my_size = 4;
        const std::size_t partner_size = 14;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0,1,2,3};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {4,5,6,7};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {8,9,10,11};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {12,13};

            neighbor_ranks = Utilities::ComputePartnerRanksAsImporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    // unordered sets can be compared, it is provided by the standard
    CHECK_EQ(exp_partner_ranks, neighbor_ranks);
}

TEST_CASE("ComputePartnerRanksAsExporter")
{
    std::set<std::size_t> exp_partner_ranks;
    std::set<std::size_t> neighbor_ranks;

    SUBCASE("serial_serial")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 1;
        exp_partner_ranks = {0};
        neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
            my_rank,
            my_size,
            partner_size);
    }

    SUBCASE("serial_distributed")
    {
        const std::size_t my_rank = 0;
        const std::size_t my_size = 1;
        const std::size_t partner_size = 5;
        exp_partner_ranks = {0};

        neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
            my_rank,
            my_size,
            partner_size);
    }

    SUBCASE("distributed_serial")
    {
        const std::size_t my_size = 4;
        const std::size_t partner_size = 1;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_same_size")
    {
        const std::size_t my_size = 2;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_1")
    {
        const std::size_t my_size = 4;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_2")
    {
        const std::size_t my_size = 7;
        const std::size_t partner_size = 2;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=4")
        {
            const std::size_t my_rank = 4;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=5")
        {
            const std::size_t my_rank = 5;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=6")
        {
            const std::size_t my_rank = 6;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_my_size_larger_3")
    {
        const std::size_t my_size = 5;
        const std::size_t partner_size = 3;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=3")
        {
            const std::size_t my_rank = 3;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=4")
        {
            const std::size_t my_rank = 4;
            exp_partner_ranks = {2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_1")
    {
        const std::size_t my_size = 2;
        const std::size_t partner_size = 4;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_2")
    {
        const std::size_t my_size = 3;
        const std::size_t partner_size = 7;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    SUBCASE("distributed_distributed_partner_size_larger_3")
    {
        const std::size_t my_size = 3;
        const std::size_t partner_size = 15;

        SUBCASE("MyRank=0")
        {
            const std::size_t my_rank = 0;
            exp_partner_ranks = {0};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=1")
        {
            const std::size_t my_rank = 1;
            exp_partner_ranks = {1};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }

        SUBCASE("MyRank=2")
        {
            const std::size_t my_rank = 2;
            exp_partner_ranks = {2};

            neighbor_ranks = Utilities::ComputePartnerRanksAsExporter(
                my_rank,
                my_size,
                partner_size);
        }
    }

    // unordered sets can be compared, it is provided by the standard
    CHECK_EQ(exp_partner_ranks, neighbor_ranks);
}

} // TEST_SUITE("Utilities")

} // namespace CoSimIO
