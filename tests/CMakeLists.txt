project(CoSimIOTesting C CXX)
cmake_minimum_required (VERSION 2.8.6)


include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )
### C++ tests ###
add_executable ( test_runner_cpp test_runner_cpp.cpp)
install( TARGETS test_runner_cpp DESTINATION ${CMAKE_SOURCE_DIR}/bin )

### C tests ###
if(BUILD_C)
    link_directories( ${CMAKE_SOURCE_DIR}/bin )

    add_executable ( test_runner_c test_runner_c.c)
    target_link_libraries(test_runner_c co_sim_io_c)

    install( TARGETS test_runner_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()


### Fortran tests ###
if(BUILD_FORTRAN)
    enable_language( Fortran )
    # include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
    # link_directories( ${CMAKE_SOURCE_DIR}/bin )

    # add_executable ( test_runner_fortran test_runner_fortran.f90)
    # # target_link_libraries(test_runner_fortran co_sim_io_c)

    # install( TARGETS test_runner_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()

