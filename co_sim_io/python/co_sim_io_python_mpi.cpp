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

// Exposure of the CoSimIO MPI interface to Python

// System includes
// pybind includes
#include <pybind11/pybind11.h>

// CoSimIO includes
#include "co_sim_io_mpi.hpp"
#include <mpi4py/mpi4py.h>


PYBIND11_MODULE(CoSimIOMPI, m)
{
    namespace py = pybind11;

    m.def("ConnectMPI", [](const CoSimIO::Info& I_Info)
        { return CoSimIO::ConnectMPI(I_Info, MPI_COMM_WORLD); });

    m.def("ConnectMPI", [](const CoSimIO::Info& I_Info, py::object py_comm)
        {
            PyObject *py_src = py_comm.ptr();

            // import the mpi4py API
            // must be done before any calls to mpi4py
            // seems to be safe to call it multiple times
            import_mpi4py();

            MPI_Comm* p_comm;

            // Check that we have been passed an mpi4py communicator
            if (PyObject_TypeCheck(py_src, &PyMPIComm_Type)) {
                // Convert to regular MPI communicator
                p_comm = PyMPIComm_Get(py_src);
            } else {
                CO_SIM_IO_ERROR << "Not an MPI_Comm passed!" << std::endl;
            }

            return CoSimIO::ConnectMPI(I_Info, MPI_COMM_WORLD);
        });
}
