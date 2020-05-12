project(CoSimIOTesting CXX)
cmake_minimum_required (VERSION 2.8.6)

include(CTest)

OPTION ( ENABLE_SOCKETS "Enable sockets communication"              OFF )
OPTION ( ENABLE_MPI     "Enable MPI communication and MPI examples" OFF )
OPTION ( BUILD_C        "Building the CoSimIO for C"                OFF )
OPTION ( BUILD_PYTHON   "Building the CoSimIO for Python"           OFF )
OPTION ( BUILD_FORTRAN  "Building the CoSimIO for Fortran"          OFF )

message("Building the CoSimIO with the following configuration:")
message("    ENABLE_SOCKETS: " ${ENABLE_SOCKETS})
message("    ENABLE_MPI:     " ${ENABLE_MPI})
message("    BUILD_TESTING:  " ${BUILD_TESTING})
message("    BUILD_C:        " ${BUILD_C})
message("    BUILD_PYTHON:   " ${BUILD_PYTHON})
message("    BUILD_FORTRAN:  " ${BUILD_FORTRAN})
message("")

if(MSVC)
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /WX ")

    set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # to suppress warning "MACOSX_RPATH is not specified for the following targets"
    set(CMAKE_MACOSX_RPATH 0)
endif()

# clean the bin directory
file(REMOVE_RECURSE bin)

if (BUILD_C)
    message( FATAL_ERROR "The CoSimIO is currently not working for C")
    add_subdirectory(co_sim_io/c)
endif()

if (BUILD_PYTHON)
    add_subdirectory(co_sim_io/python)
endif()

if (BUILD_FORTRAN)
    # TODO since fortran relies on C, a check is needed
    message( FATAL_ERROR "The CoSimIO is currently not working for Fortran")
    add_subdirectory(co_sim_io/fortran)
endif()

add_subdirectory(examples)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

configure_file(
  "tests/compiled_config.json.in"
  "${CMAKE_SOURCE_DIR}/tests/compiled_config.json"
)
