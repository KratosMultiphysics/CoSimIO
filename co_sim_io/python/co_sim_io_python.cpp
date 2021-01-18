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

// CoSimIO includes
#include "../co_sim_io.hpp"
#include "info_to_python.hpp"
#include "model_part_to_python.hpp"
#include "vector_to_python.hpp"
#include "connection_status_to_python.hpp"
#include "version_to_python.hpp"

namespace CoSimIO_Py_Wrappers {

using ImportMeshReturnType = std::tuple<
    CoSimIO::Info,
    std::vector<double>,
    std::vector<int>,
    std::vector<int>>;

using ImportDataReturnType = std::tuple<
    CoSimIO::Info,
    std::vector<double>>;

CoSimIO::Info ImportMesh(
    const CoSimIO::Info& I_Info,
    CoSimIO::ModelPart& rModelPart)
{
    return CoSimIO::ImportMesh(
        I_Info,
        rModelPart);
}

CoSimIO::Info ExportMesh(
    const CoSimIO::Info& I_Info,
    const CoSimIO::ModelPart& rModelPart)
{
    return CoSimIO::ExportMesh(
        I_Info,
        rModelPart);
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

} // namespace CoSimIO_Py_Wrappers


PYBIND11_MODULE(CoSimIO, m)
{
    namespace py = pybind11;

    m.def("Hello", & CoSimIO::Hello);

    m.def("Connect",    &CoSimIO::Connect);
    m.def("Disconnect", &CoSimIO::Disconnect);

    m.def("ImportMesh", CoSimIO_Py_Wrappers::ImportMesh);
    m.def("ExportMesh", CoSimIO_Py_Wrappers::ExportMesh);
    // TODO uncomment when starting to use new Interface
    // m.def("ImportMesh", &CoSimIO::ImportMesh);
    // m.def("ExportMesh", &CoSimIO::ExportMesh);

    m.def("ImportData", CoSimIO_Py_Wrappers::ImportData);
    m.def("ExportData", CoSimIO_Py_Wrappers::ExportData);

    m.def("ImportInfo", &CoSimIO::ImportInfo);
    m.def("ExportInfo", &CoSimIO::ExportInfo);

    // functions for CoSim-orchestrated CoSimulation
    m.def("Run", &CoSimIO::Run);

    m.def("Register", [](
        const CoSimIO::Info& I_Info,
        std::function<CoSimIO::Info(const CoSimIO::Info&)> FunctionPointer)
        { return CoSimIO::Register(I_Info, FunctionPointer); } );

    AddCoSimIOInfoToPython(m);
    AddCoSimIOModelPartToPython(m);
    AddCoSimIOVectorToPython(m);
    AddCoSimIOConnectionStatusToPython(m);
    AddCoSimIOVersionToPython(m);
}
