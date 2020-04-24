project(CoSimIOTesting CXX)
cmake_minimum_required (VERSION 2.8.6)

OPTION ( ENABLE_SOCKETS "Enable sockets communication" OFF )
OPTION ( ENABLE_MPI "Enable MPI communication and MPI examples" OFF )
OPTION ( BUILD_TESTS "Enable MPI communication and MPI examples" ON )
OPTION ( BUILD_C "Enable MPI communication and MPI examples" OFF )
OPTION ( BUILD_PYTHON "Enable MPI communication and MPI examples" OFF )
OPTION ( BUILD_FORTRAN "Enable MPI communication and MPI examples" OFF )

message("\nUsing the following configuration:")
message("    ENABLE_SOCKETS:    " ${ENABLE_SOCKETS})
message("    ENABLE_MPI:        " ${ENABLE_MPI})
message("    BUILD_TESTS:       " ${BUILD_TESTS})
message("    BUILD_C:           " ${BUILD_C})
message("    BUILD_PYTHON:      " ${BUILD_PYTHON})
message("    BUILD_FORTRAN:     " ${BUILD_FORTRAN})
message("")

if(MSVC)
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus ") # /WX
endif()

# always build the C++ simple solver
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/cpp)

if (BUILD_C)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/c)
endif()

if (BUILD_PYTHON)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/python)
endif()

if (BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
