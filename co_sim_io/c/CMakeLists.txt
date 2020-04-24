project(CoSimCIO C CXX)
cmake_minimum_required (VERSION 2.8.6)

message("Configuring CoSimIO for C")

set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

if(MSVC)
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

add_library (co_sim_io_c SHARED co_sim_io_c.cpp)

# enable C++11
set_target_properties(co_sim_io_c PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS co_sim_io_c DESTINATION ${CMAKE_SOURCE_DIR}/bin)
