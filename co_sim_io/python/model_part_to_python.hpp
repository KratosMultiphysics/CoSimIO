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

#ifndef CO_SIM_IO_MODEL_PART_TO_PYHON_INCLUDED
#define CO_SIM_IO_MODEL_PART_TO_PYHON_INCLUDED

// Exposure of the CoSimIO to Python

// System includes
#include <string>
#include <sstream>

// pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// CoSimIO include
#include "../impl/model_part.hpp"

void AddCoSimIOModelPartToPython(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<CoSimIO::Node, CoSimIO::ModelPart::NodePointerType>(m,"Node")
        .def(py::init<const CoSimIO::IdType, const double, const double, const double>())
        .def(py::init<const CoSimIO::IdType, const CoSimIO::CoordinatesType&>())
        .def("Id", &CoSimIO::Node::Id)
        .def("X",  &CoSimIO::Node::X)
        .def("Y",  &CoSimIO::Node::Y)
        .def("Z",  &CoSimIO::Node::Z)
        .def("Coordinates",  &CoSimIO::Node::Coordinates)
        .def("__str__",   [](const CoSimIO::Node& I_Node)
            { std::stringstream ss; ss << I_Node; return ss.str(); } )
        ;

    py::class_<CoSimIO::Element, CoSimIO::ModelPart::ElementPointerType>(m,"Element")
        .def(py::init<const CoSimIO::IdType, const CoSimIO::ElementType, const CoSimIO::Element::NodesContainerType&>())
        .def("Id", &CoSimIO::Element::Id)
        .def("Type", &CoSimIO::Element::Type)
        .def("NumberOfNodes", &CoSimIO::Element::NumberOfNodes)
        .def_property_readonly("Nodes", [](CoSimIO::Element& I_Element) {
            return py::make_iterator(I_Element.NodesBegin(), I_Element.NodesEnd());
            }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def("__str__",   [](const CoSimIO::Element& I_Element)
            { std::stringstream ss; ss << I_Element; return ss.str(); } )
        ;

    py::class_<CoSimIO::ModelPart>(m,"ModelPart")
        .def(py::init<const std::string&>())
        .def("Name",             &CoSimIO::ModelPart::Name)
        .def("NumberOfNodes",    &CoSimIO::ModelPart::NumberOfNodes)
        .def("NumberOfElements", &CoSimIO::ModelPart::NumberOfElements)
        .def("CreateNewNode",    &CoSimIO::ModelPart::CreateNewNode, py::return_value_policy::reference_internal)
        .def("CreateNewElement", &CoSimIO::ModelPart::CreateNewElement, py::return_value_policy::reference_internal)
        .def("GetNode",          [](CoSimIO::ModelPart& I_ModelPart, const CoSimIO::IdType I_Id){
            return I_ModelPart.pGetNode(I_Id);}, py::return_value_policy::reference_internal)
        .def("GetElement",       [](CoSimIO::ModelPart& I_ModelPart, const CoSimIO::IdType I_Id){
            return I_ModelPart.pGetElement(I_Id);}, py::return_value_policy::reference_internal)
        .def_property_readonly("Nodes", [](CoSimIO::ModelPart& I_ModelPart) {
            return py::make_iterator(I_ModelPart.NodesBegin(), I_ModelPart.NodesEnd());
            }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def_property_readonly("Elements", [](CoSimIO::ModelPart& I_ModelPart) {
            return py::make_iterator(I_ModelPart.ElementsBegin(), I_ModelPart.ElementsEnd());
            }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def("__str__",   [](const CoSimIO::ModelPart& I_ModelPart)
            { std::stringstream ss; ss << I_ModelPart; return ss.str(); } )
        ;

    py::enum_<CoSimIO::ElementType>(m,"ElementType")
        .value("VERTEX",     CoSimIO::ElementType::VERTEX)
        .value("LINE",       CoSimIO::ElementType::LINE)
        .value("TRIANGLE",   CoSimIO::ElementType::TRIANGLE)
        .value("QUAD",       CoSimIO::ElementType::QUAD)
        .value("TETRA",      CoSimIO::ElementType::TETRA)
        .value("HEXAHEDRON", CoSimIO::ElementType::HEXAHEDRON)
        ;
}

#endif // CO_SIM_IO_MODEL_PART_TO_PYHON_INCLUDED
