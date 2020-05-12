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

using ImportMeshReturnType = std::tuple<
    CoSimIO::Info,
    std::vector<double>,
    std::vector<int>,
    std::vector<int>>;

using ImportDataReturnType = std::tuple<
    CoSimIO::Info,
    std::vector<double>>;

ImportMeshReturnType ImportMesh(const CoSimIO::Info& I_Info)
{
    std::vector<double> nodal_coordinates;
    std::vector<int> element_connectivities;
    std::vector<int> element_types;

    auto info = CoSimIO::ImportMesh(
        I_Info,
        nodal_coordinates,
        element_connectivities,
        element_types);

    return std::make_tuple(info, nodal_coordinates, element_connectivities, element_types);
}

CoSimIO::Info ExportMesh(
    const CoSimIO::Info& I_Info,
    std::vector<double>& rNodalCoordinates,
    std::vector<int>& rElementConnectivities,
    std::vector<int>& rElementTypes)
{
    return CoSimIO::ExportMesh(
        I_Info,
        rNodalCoordinates,
        rElementConnectivities,
        rElementTypes);
}

ImportDataReturnType ImportData(
    const CoSimIO::Info& I_Info)
{
    std::vector<double> values;

    auto info = CoSimIO::ImportData(
        I_Info,
        values);

    return std::make_tuple(info, values);
}

CoSimIO::Info ExportData(
    const CoSimIO::Info& I_Info,
    std::vector<double>& rValues)
{
    return CoSimIO::ExportData(
        I_Info,
        rValues);
}

// This function is used to print the ofstream-operator
// i.e. printing an object will give the same result in Python as in C++
// To be defined as the "__str__" function
// e.g. ".def("__str__", PrintObject<ProcessInfo>)"
// It replicates the function "self_ns::str(self))" of boost-python
template< class T>
std::string PrintObject(const T& rObject)
{
    std::stringstream ss;
    ss << rObject;
    return ss.str();
}

} // namespace CoSimIO_Py_Wrappers


PYBIND11_MODULE(CoSimIO, m)
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
        .def("__str__",   CoSimIO_Py_Wrappers::PrintObject<CoSimIO::Info>);
        ;

    m.def("Hello", & CoSimIO::Hello);

    m.def("Connect", &CoSimIO::Connect);
    m.def("Disconnect", &CoSimIO::Disconnect);

    m.def("IsConverged", &CoSimIO::IsConverged);

    m.def("ImportMesh", CoSimIO_Py_Wrappers::ImportMesh);
    m.def("ExportMesh", CoSimIO_Py_Wrappers::ExportMesh);

    m.def("ImportData", CoSimIO_Py_Wrappers::ImportData);
    m.def("ExportData", CoSimIO_Py_Wrappers::ExportData);

    // functions for CoSim-orchestrated CoSimulation
    m.def("Run", &CoSimIO::Run);

    m.def("Register", [](
        const CoSimIO::Info& I_Info,
        std::function<CoSimIO::Info(const CoSimIO::Info&)> FunctionPointer)
        { return CoSimIO::Register(I_Info, FunctionPointer); } );

    py::enum_<CoSimIO::ConnectionStatus>(m,"ConnectionStatus")
        .value("NotConnected",CoSimIO::ConnectionStatus::NotConnected)
        .value("Connected",CoSimIO::ConnectionStatus::Connected)
        .value("Disconnected", CoSimIO::ConnectionStatus::Disconnected)
        .value("ConnectionError", CoSimIO::ConnectionStatus::ConnectionError)
        .value("DisconnectionError", CoSimIO::ConnectionStatus::DisconnectionError)
        ;


    std::stringstream version_stream;
    version_stream << CoSimIO::GetMajorVersion() << "."
                   << CoSimIO::GetMinorVersion() << "."
                   << CoSimIO::GetPatchVersion();
    m.attr("__version__") = version_stream.str();
}
