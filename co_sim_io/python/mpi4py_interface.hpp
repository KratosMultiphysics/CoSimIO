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

#ifndef CO_SIM_IO_MPI4PY_COMM_TO_PYTHON_INCLUDED
#define CO_SIM_IO_MPI4PY_COMM_TO_PYTHON_INCLUDED

// System includes
#include <stdexcept>

// External includes
#include <pybind11/pybind11.h>
#include <mpi4py/mpi4py.h>

// Project includes
#include "mpi_comm_holder.hpp"


namespace CoSimIO {

namespace py = pybind11;

class mpi4pyCommHolder : public MPICommHolder
{
public:
    mpi4pyCommHolder(py::object PyComm) : MPICommHolder(GetMPICommFromPybindObject(PyComm)) {}

private:
    MPI_Comm GetMPICommFromPybindObject(py::object PyComm)
    {
        PyObject *py_src = PyComm.ptr();

        // import the mpi4py API
        // must be done before any calls to mpi4py
        // seems to be safe to call it multiple times
        import_mpi4py();

        // Check that we have been passed an mpi4py communicator
        if (PyObject_TypeCheck(py_src, &PyMPIComm_Type)) {
            // Convert to regular MPI communicator
            return *(PyMPIComm_Get(py_src));
        } else {
            throw std::runtime_error("A mpi4py Communictor object must be passed!");
        }
    }
};


void AddMPI4PyInterface(pybind11::module& m)
{
    py::class_<mpi4pyCommHolder, std::shared_ptr<mpi4pyCommHolder>, MPICommHolder>(m,"mpi4pyCommHolder")
        .def(py::init<py::object>())
        ;
}

} // namespace CoSimIO

#endif // CO_SIM_IO_MPI4PY_COMM_TO_PYTHON_INCLUDED
