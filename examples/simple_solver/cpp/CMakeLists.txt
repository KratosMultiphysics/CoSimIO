project( SimpleSolverCpp CXX )
cmake_minimum_required (VERSION 2.8.6)

if(${MSVC})
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../co_sim_io )

add_executable (simple_solver_cpp ${CMAKE_CURRENT_SOURCE_DIR}/simple_solver.cpp)

# enable C++11
set_target_properties(simple_solver_cpp PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS simple_solver_cpp DESTINATION ${CMAKE_SOURCE_DIR}/bin )
