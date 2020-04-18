project(CoSimIOTesting CXX)
cmake_minimum_required (VERSION 2.8.6)

include(CMakeDependentOption)
OPTION ( CO_SIM_IO_SOCKETS "Enable sockets communication" OFF )
OPTION ( CO_SIM_IO_USING_MPI "Enable MPI communication and MPI examples" OFF )
cmake_dependent_option(ENABLE_FORTRAN_EXAMPLES "Enable Fortran example" OFF MSVC ON) # disable by default in Win, enable otherwise

message("\nUsing the following configuration:")
message("    CO_SIM_IO_SOCKETS:        " ${CO_SIM_IO_SOCKETS})
message("    CO_SIM_IO_USING_MPI:      " ${CO_SIM_IO_USING_MPI})
message("    ENABLE_FORTRAN_EXAMPLES:  " ${ENABLE_FORTRAN_EXAMPLES})
message("")

if(MSVC)
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

if(CO_SIM_IO_USING_MPI)
    add_definitions( -DCO_SIM_IO_USING_MPI )
    find_package(MPI REQUIRED)
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
endif()

if(CO_SIM_IO_SOCKETS)
    add_definitions(-DCO_SIM_IO_USING_SOCKETS)

    # the following line is required for older versions of boost, see:
    # https://stackoverflow.com/questions/9723793/undefined-reference-to-boostsystemsystem-category-when-compiling
    add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

    add_definitions(-DBOOST_DATE_TIME_NO_LIB)
    add_definitions(-DBOOST_REGEX_NO_LIB)

    include_directories(SYSTEM external_libraries/boost_1_71_0) # FIXME
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/c)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/python)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/c)
if(CO_SIM_IO_USING_MPI)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/cpp)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/c)
endif()

if(ENABLE_FORTRAN_EXAMPLES)
    include(CMakeAddFortranSubdirectory)
    cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/fortran NO_EXTERNAL_INSTALL)
    if(CO_SIM_IO_USING_MPI)
        cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/fortran NO_EXTERNAL_INSTALL)
    endif()
endif()
