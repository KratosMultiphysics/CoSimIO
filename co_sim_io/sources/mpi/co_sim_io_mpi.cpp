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

/*
This file contains the implementation of the functions defined in "co_sim_io_mpi.hpp"
*/

// System includes

// Project includes
#include "../../co_sim_io_mpi.hpp"

namespace CoSimIO {

Info ConnectMPI(
    const Info& I_Settings,
    MPI_Comm ThisMPIComm)
{
    CO_SIM_IO_ERROR << "Not implemented!" << std::endl;

    // make sure MPI was already initialized by the host
    int flag_initialized;
    MPI_Initialized(&flag_initialized);
    CO_SIM_IO_ERROR_IF_NOT(flag_initialized) << "MPI must be initialized before calling \"ConnectMPI\"!" << std::endl;

    return Info(); // TODO use this
}

} // namespace CoSimIO
