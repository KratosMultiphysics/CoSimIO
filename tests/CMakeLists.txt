project(CoSimIOTesting C CXX)
cmake_minimum_required (VERSION 2.8.6)

include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )
include_directories( ${CMAKE_SOURCE_DIR}/external_libraries )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

### Test runner for tests of CoSimIO
file( GLOB_RECURSE CO_SIM_IO_TEST_FILES co_sim_io/*.cpp)
add_executable ( test_runner_co_sim_io_tests ${CO_SIM_IO_TEST_FILES})
install( TARGETS test_runner_co_sim_io_tests DESTINATION ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(test_runner_co_sim_io_tests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


### C++ tests ###
add_executable ( test_runner_cpp test_runners/test_runner.cpp)
install( TARGETS test_runner_cpp DESTINATION ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(test_runner_cpp PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

### C tests ###
if(BUILD_C)
    link_directories( ${CMAKE_SOURCE_DIR}/bin )

    add_executable ( test_runner_c test_runners/test_runner.c)
    target_link_libraries(test_runner_c co_sim_io_c)

    install( TARGETS test_runner_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()


### Fortran tests ###
if(BUILD_FORTRAN)
    enable_language( Fortran )
    # include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
    # link_directories( ${CMAKE_SOURCE_DIR}/bin )

    # TODO glob all fortran files
    # add_executable ( test_runner_fortran test_runners/test_runner_fortran.f90)
    # # target_link_libraries(test_runner_fortran co_sim_io_c)

    # install( TARGETS test_runner_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()

