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

// Exposure of the CoSimIO to Python

// System includes
#include <functional>
#include <vector>
#include <string>
#include <tuple>

// pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

// CoSimIO include
#include "../co_sim_io.hpp"

namespace CoSimIO_Py_Wrappers {

std::tuple<std::vector<double>, std::vector<int>, std::vector<int>> ImportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier)
{
    std::vector<double> nodal_coordinates;
    std::vector<int> element_connectivities;
    std::vector<int> element_types;

    CoSimIO::ImportMesh(
        rConnectionName,
        rIdentifier,
        nodal_coordinates,
        element_connectivities,
        element_types);

    return std::make_tuple(nodal_coordinates, element_connectivities, element_types);
}

void ExportMesh(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    std::vector<double>& rNodalCoordinates,
    std::vector<int>& rElementConnectivities,
    std::vector<int>& rElementTypes)
{
    CoSimIO::ExportMesh(
        rConnectionName,
        rIdentifier,
        rNodalCoordinates,
        rElementConnectivities,
        rElementTypes);
}

std::vector<double> ImportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier)
{
    std::vector<double> values;

    CoSimIO::ImportData(
        rConnectionName,
        rIdentifier,
        values);

    return values;
}

void ExportData(
    const std::string& rConnectionName,
    const std::string& rIdentifier,
    std::vector<double>& rValues)
{
    CoSimIO::ExportData(
        rConnectionName,
        rIdentifier,
        rValues);
}

} // namespace CoSimIO_Py_Wrappers


PYBIND11_MODULE(CoSimIO, m)
{
    namespace py = pybind11;

    CoSimIO::ReturnInfo (*ConnectWithSettingsFileName)(const std::string&, const std::string&) = &CoSimIO::Connect;
    CoSimIO::ReturnInfo (*ConnectWithSettings)(const std::string&, CoSimIO::SettingsType) = &CoSimIO::Connect;

    m.def("Connect", ConnectWithSettingsFileName);
    m.def("Connect", ConnectWithSettings);

    m.def("Disconnect", &CoSimIO::Disconnect);

    m.def("IsConverged", &CoSimIO::IsConverged);

    m.def("ImportMesh", CoSimIO_Py_Wrappers::ImportMesh);
    m.def("ExportMesh", CoSimIO_Py_Wrappers::ExportMesh);

    m.def("ImportData", CoSimIO_Py_Wrappers::ImportData);
    m.def("ExportData", CoSimIO_Py_Wrappers::ExportData);

    // functions for CoSim-orchestrated CoSimulation
    m.def("Run", &CoSimIO::Run);

    m.def("Register", [](
        const std::string& rConnectionName,
        const std::string& rFunctionName,
        std::function<void(CoSimIO::Info&)> FunctionPointer)
        { CoSimIO::Register(rConnectionName, rFunctionName, FunctionPointer); } );


    py::class_<CoSimIO::Info>(m,"Info")
        .def(py::init<>())
        .def("Has",&CoSimIO::Info::Has)
        .def("GetInt",    &CoSimIO::Info::Get<int>)
        .def("GetDouble", &CoSimIO::Info::Get<double>)
        .def("GetBool",   &CoSimIO::Info::Get<bool>)
        .def("GetString", &CoSimIO::Info::Get<std::string>)
        .def("SetInt",    &CoSimIO::Info::Set<int>)
        .def("SetDouble", &CoSimIO::Info::Set<double>)
        .def("SetBool",   &CoSimIO::Info::Set<bool>)
        .def("SetString", &CoSimIO::Info::Set<std::string>)
        ;
}
