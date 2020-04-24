project(CoSimIOTesting C CXX Fortran)
cmake_minimum_required (VERSION 2.8.6)


### C++ tests ###


### C tests ###
if(BUILD_C)
include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/c )
link_directories( ${CMAKE_SOURCE_DIR}/bin )

add_executable ( test_runner_c test_runner_c.c)
# target_link_libraries(simple_solver_c co_sim_io_c)

install( TARGETS test_runner_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()


### Fortran tests ###
if(BUILD_FORTRAN)
    include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
    link_directories( ${CMAKE_SOURCE_DIR}/bin )

    add_executable ( test_runner_fortran test_runner_fortran.f90)
    # target_link_libraries(test_runner_fortran co_sim_io_c)

    install( TARGETS test_runner_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()

