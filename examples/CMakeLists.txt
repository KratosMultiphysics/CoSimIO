project(CoSimIOTesting CXX)
cmake_minimum_required (VERSION 2.8.6)


if(ENABLE_MPI)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/cpp)
endif()


if (BUILD_C)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simple_solver/c)
    if(ENABLE_MPI)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/c)
    endif()
endif()


if(BUILD_FORTRAN)
    include(CMakeAddFortranSubdirectory)
    cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simple_solver/fortran NO_EXTERNAL_INSTALL)
    if(ENABLE_MPI)
        cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/fortran NO_EXTERNAL_INSTALL)
    endif()
endif()