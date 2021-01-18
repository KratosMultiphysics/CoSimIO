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

#ifndef CO_SIM_IO_VECTOR_TO_PYHON_INCLUDED
#define CO_SIM_IO_VECTOR_TO_PYHON_INCLUDED

// Exposure of the CoSimIO to Python

// System includes
#include <vector>
#include <sstream>

// pybind includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using VEC_DataType = double;
using VEC_VectorType = std::vector<VEC_DataType>;

PYBIND11_MAKE_OPAQUE(VEC_VectorType)

void AddCoSimIOVectorToPython(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<VEC_VectorType>(m,"Vector")
        .def(py::init<>())
        .def(py::init<const std::vector<VEC_DataType>&>())
        .def(py::init<const VEC_VectorType&>())
        .def("__len__", [](VEC_VectorType& v)
            { return v.size(); } )
        .def("size", [](VEC_VectorType& v)
            { return v.size(); } )
        .def("resize", [](VEC_VectorType& v, const std::size_t& size)
            { v.resize(size); } )
        .def("append", [](VEC_VectorType& v, const VEC_DataType& val)
            { v.push_back(val); } )
        .def("__setitem__", [](VEC_VectorType& v, const std::size_t I_Index, const VEC_DataType& val)
            { v[I_Index] = val; } )
        .def("__getitem__", [](VEC_VectorType& v, const std::size_t I_Index)
            { return v[I_Index]; } )
        .def("__iter__",     [](VEC_VectorType& v)
            {return py::make_iterator(v.begin(), v.end());},  py::keep_alive<0,1>())
        .def("__str__",   [](const VEC_VectorType& v)
            {
                std::stringstream ss;
                const std::size_t size = v.size();

                ss << "[";
                if(size>0) ss << v[0];
                if(size>1) {
                    for(std::size_t i=1; i<size; ++i)
                        ss<<", "<<v[i];
                }
                ss << "]";

                return ss.str(); } )
        ;
}

#endif // CO_SIM_IO_VECTOR_TO_PYHON_INCLUDED
