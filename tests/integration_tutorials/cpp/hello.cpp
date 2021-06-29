//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Pooyan Dadvand
//

// CoSimulation includes
#include "co_sim_io.hpp"
DEFINE_SINGLETON_MAIN( );

#define COSIMIO_CHECK_EQUAL(a, b)                                \
    if (a != b) {                                                \
        std::cout << "in line " << __LINE__ << " : " << a        \
                  << " is not equal to " << b << std::endl;      \
        return 1;                                                \
    }

int main()
{
    auto info = CoSimIO::Hello();

    std::cout << info << std::endl;

    int major_version = info.Get<int>("major_version");
    int minor_version = info.Get<int>("minor_version");
    std::string patch_version = info.Get<std::string>("patch_version");

    COSIMIO_CHECK_EQUAL((major_version>0), 1);
    COSIMIO_CHECK_EQUAL((minor_version>=0), 1);

    return 0;
}
