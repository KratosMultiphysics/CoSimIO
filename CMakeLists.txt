project(CoSimIOTesting CXX)
cmake_minimum_required (VERSION 2.8.6)

include(CMakeDependentOption)
OPTION ( ENABLE_SOCKETS "Enable sockets communication" OFF )
OPTION ( ENABLE_MPI "Enable MPI communication and MPI examples" OFF )
cmake_dependent_option(ENABLE_FORTRAN_EXAMPLES "Enable Fortran example" OFF MSVC ON) # disable by default in Win, enable otherwise

message("\nUsing the following configuration:")
message("    ENABLE_SOCKETS:          " ${ENABLE_SOCKETS})
message("    ENABLE_MPI:              " ${ENABLE_MPI})
message("    ENABLE_FORTRAN_EXAMPLES: " ${ENABLE_FORTRAN_EXAMPLES})
message("")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/c)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/python)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/c)
if(ENABLE_MPI)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/cpp)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/c)
endif()

if(ENABLE_FORTRAN_EXAMPLES)
    include(CMakeAddFortranSubdirectory)
    cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_solver/fortran NO_EXTERNAL_INSTALL)
    if(ENABLE_MPI)
        cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/mpi_solver/fortran NO_EXTERNAL_INSTALL)
    endif()
endif()
