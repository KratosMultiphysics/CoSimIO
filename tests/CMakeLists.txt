include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )
include_directories( ${CMAKE_SOURCE_DIR}/external_libraries )

### Test runner for tests of CoSimIO
file( GLOB_RECURSE CO_SIM_IO_TEST_FILES co_sim_io/*.cpp)
add_executable ( test_runner_co_sim_io_tests ${CO_SIM_IO_TEST_FILES})
install( TARGETS test_runner_co_sim_io_tests DESTINATION ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(test_runner_co_sim_io_tests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(co_sim_io_tests test_runner_co_sim_io_tests)


### C++ tests ###
function(add_cpp_executable TEST_SOURCE_FILE)
    get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
    string(REGEX REPLACE ".cpp" "_cpp_test" TEST_NAME ${TEST_FILENAME})
    message(STATUS  "adding ${TEST_NAME} ")
    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
    install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_cpp")
endfunction()

file(GLOB cpp_test_files "solver_integration/cpp_solver/*.cpp")

foreach(cpp_test_file ${cpp_test_files})
add_cpp_executable(${cpp_test_file})
endforeach(cpp_test_file)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/run.sh  $1 & $2)

add_test(hello_cpp_test hello_cpp_test)
add_test(NAME connect_disconnect_cpp_test COMMAND bash run.sh $<TARGET_FILE:connect_disconnect_cpp_test>  $<TARGET_FILE:connect_disconnect_cpp_test>)
add_test(NAME import_export_data_cpp_test COMMAND bash run.sh $<TARGET_FILE:export_data_cpp_test>  $<TARGET_FILE:import_data_cpp_test>)
add_test(NAME import_export_mesh_cpp_test COMMAND bash run.sh $<TARGET_FILE:export_mesh_cpp_test>  $<TARGET_FILE:import_mesh_cpp_test>)

### C tests ###
if(BUILD_C)
    function(add_c_test TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE "[.]c" "_c_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_c)
        add_test(${TEST_NAME} ${TEST_NAME})
        install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_c")
    endfunction()

    function(add_c_executable TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE "[.]c" "_c_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_c)
        install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_c")
    endfunction()

    file(GLOB c_test_files "co_sim_io/c/*.c")

    foreach(c_test_file ${c_test_files})
    add_c_test(${c_test_file})
    endforeach(c_test_file)

    file(GLOB c_integration_test_files "solver_integration/c/*.c")

    foreach(c_integration_test_file ${c_integration_test_files})
    add_c_executable(${c_integration_test_file})
    endforeach(c_integration_test_file)

    add_test(hello_c_test hello_c_test)
    add_test(NAME connect_disconnect_c_test COMMAND bash run.sh $<TARGET_FILE:connect_disconnect_c_test>  $<TARGET_FILE:connect_disconnect_c_test>)
    add_test(NAME import_export_data_c_test COMMAND bash run.sh $<TARGET_FILE:export_data_c_test>  $<TARGET_FILE:import_data_c_test>)
endif()


### Fortran tests ###
if(BUILD_FORTRAN)
    enable_language( Fortran )
    include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
    link_directories( ${CMAKE_SOURCE_DIR}/bin )

    # file( GLOB_RECURSE FORTRAN_TEST_FILES *.f90)
    # add_executable ( test_runner_fortran test_runners/test_runner.f90 ${FORTRAN_TEST_FILES})
    # target_link_libraries(test_runner_fortran co_sim_io_c)

    # install( TARGETS test_runner_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin )
endif()
