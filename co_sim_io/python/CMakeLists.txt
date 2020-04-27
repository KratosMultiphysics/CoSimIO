project(CoSimPythonIO CXX)
cmake_minimum_required (VERSION 2.8.6)

message("Configuring CoSimIO for Python")

if(${MSVC})
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

pybind11_add_module(CoSimIO co_sim_io_python.cpp)

# enable C++11
set_target_properties(CoSimIO PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS CoSimIO DESTINATION ${CMAKE_SOURCE_DIR}/tests)
