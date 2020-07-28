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

#ifndef CO_SIM_IO_INFO_TO_PYHON_INCLUDED
#define CO_SIM_IO_INFO_TO_PYHON_INCLUDED

// Exposure of the CoSimIO to Python

// System includes
#include <string>
#include <sstream>

// pybind includes
#include <pybind11/pybind11.h>

// CoSimIO include
#include "../co_sim_io.hpp"

void AddCoSimIOInfoToPython(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<CoSimIO::Info>(m,"Info")
    .def(py::init<>())
    .def("Has",       &CoSimIO::Info::Has)
    .def("GetInt",    &CoSimIO::Info::Get<int>)
    .def("GetDouble", &CoSimIO::Info::Get<double>)
    .def("GetBool",   &CoSimIO::Info::Get<bool>)
    .def("GetString", &CoSimIO::Info::Get<std::string>)
    .def("SetInt",    &CoSimIO::Info::Set<int>)
    .def("SetDouble", &CoSimIO::Info::Set<double>)
    .def("SetBool",   &CoSimIO::Info::Set<bool>)
    .def("SetString", &CoSimIO::Info::Set<std::string>)
    .def("Erase",     &CoSimIO::Info::Erase)
    .def("Clear",     &CoSimIO::Info::Clear)
    .def("Size",      &CoSimIO::Info::Size)
    .def("__str__",   [](const CoSimIO::Info& I_Info)
        { std::stringstream ss; ss << I_Info; return ss.str(); } );
    ;
}

#endif // CO_SIM_IO_INFO_TO_PYHON_INCLUDED
