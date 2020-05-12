include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )
link_directories( ${CMAKE_SOURCE_DIR}/bin )

# always build the C++ simple solver
add_executable(simple_solver_cpp ${CMAKE_CURRENT_SOURCE_DIR}/simple_solver/cpp/solver.cpp)
install(TARGETS simple_solver_cpp DESTINATION ${CMAKE_SOURCE_DIR}/bin )

if(ENABLE_MPI)
    add_executable (mpi_solver_cpp ${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/cpp/solver.cpp)
    install(TARGETS mpi_solver_cpp DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()

if (BUILD_C)
    add_executable(simple_solver_c ${CMAKE_CURRENT_SOURCE_DIR}/simple_solver/c/solver.c)
    target_link_libraries(simple_solver_c co_sim_io_c)
    install(TARGETS simple_solver_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
    if(ENABLE_MPI)
        add_executable(mpi_solver_c ${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/c/solver.c)
        target_link_libraries(mpi_solver_c co_sim_io_c)
        install(TARGETS mpi_solver_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
    endif()
endif()


# if(BUILD_FORTRAN)
#     include(CMakeAddFortranSubdirectory)
#     cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simple_solver/fortran NO_EXTERNAL_INSTALL)
#     if(ENABLE_MPI)
#         cmake_add_fortran_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mpi_solver/fortran NO_EXTERNAL_INSTALL)
#     endif()
# endif()