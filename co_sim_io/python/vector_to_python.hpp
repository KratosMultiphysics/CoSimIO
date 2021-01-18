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

namespace {

// small wrapper around std::vector for using it in Python
template<typename TDataType>
class VectorWrapper
{
public:
    VectorWrapper() = default;
    VectorWrapper(const std::vector<TDataType>& I_Vector)
        : mVector(I_Vector) {};

    VectorWrapper(const VectorWrapper& Other) : mVector(Other.mVector) {}
    VectorWrapper& operator=(const VectorWrapper&) = delete;

    std::vector<TDataType>& Vector() {return mVector;}
    const std::vector<TDataType>& Vector() const {return mVector;}

private:
    std::vector<TDataType> mVector;
};
}

void AddCoSimIOVectorToPython(pybind11::module& m)
{
    namespace py = pybind11;

    using DataType = double;
    using VectorType = VectorWrapper<DataType>;

    py::class_<VectorType>(m,"Vector")
        .def(py::init<>())
        .def(py::init<const std::vector<DataType>&>())
        .def(py::init<const VectorType&>())
        .def("__len__", [](VectorType& I_Vec)
            { return I_Vec.Vector().size(); } )
        .def("size", [](VectorType& I_Vec)
            { return I_Vec.Vector().size(); } )
        .def("resize", [](VectorType& I_Vec, const std::size_t& I_NewSize)
            { I_Vec.Vector().resize(I_NewSize); } )
        .def("append", [](VectorType& I_Vec, const DataType& I_Val)
            { I_Vec.Vector().push_back(I_Val); } )
        .def("__setitem__", [](VectorType& I_Vec, const std::size_t I_Index, const DataType& I_Val)
            { I_Vec.Vector()[I_Index] = I_Val; } )
        .def("__getitem__", [](VectorType& I_Vec, const std::size_t I_Index)
            { return I_Vec.Vector()[I_Index]; } )
        .def("__iter__",     [](VectorType& I_Vec)
            {return py::make_iterator(I_Vec.Vector().begin(), I_Vec.Vector().end());},  py::keep_alive<0,1>())
        .def("__str__",   [](const VectorType& I_Vec)
            {
                std::stringstream ss;
                const std::size_t size = I_Vec.Vector().size();

                ss << "[";
                if(size>0) ss << I_Vec.Vector()[0];
                if(size>1) {
                    for(std::size_t i=1; i<size; ++i)
                        ss<<", "<<I_Vec.Vector()[i];
                }
                ss << "]";

                return ss.str(); } )
        ;
}

#endif // CO_SIM_IO_VECTOR_TO_PYHON_INCLUDED
