include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )
include_directories( ${CMAKE_SOURCE_DIR}/external_libraries )


### Test runner for tests of CoSimIO
find_package (Threads REQUIRED)
file( GLOB CO_SIM_IO_TEST_FILES co_sim_io/impl/*.cpp)
add_executable ( co_sim_io_tests ${CO_SIM_IO_TEST_FILES})
target_link_libraries(co_sim_io_tests co_sim_io ${CMAKE_THREAD_LIBS_INIT})
install( TARGETS co_sim_io_tests DESTINATION ${CMAKE_SOURCE_DIR}/bin )
set_target_properties(co_sim_io_tests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


doctest_discover_tests(co_sim_io_tests WORKING_DIRECTORY $<TARGET_FILE_DIR:co_sim_io>)

if (CO_SIM_IO_ENABLE_MPI)
    file( GLOB CO_SIM_IO_MPI_TEST_FILES co_sim_io/impl/mpi/*.cpp )
    add_executable ( co_sim_io_mpi_tests ${CO_SIM_IO_MPI_TEST_FILES} )
    target_link_libraries( co_sim_io_mpi_tests co_sim_io_mpi ${MPI_LIBRARIES} )
    install( TARGETS co_sim_io_mpi_tests DESTINATION ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties(co_sim_io_mpi_tests PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    add_test(NAME "MPI-C++-tests-4-Cores" COMMAND mpiexec -np 4 co_sim_io_mpi_tests)

endif()

### C++ tests ###
function(add_cpp_executable TEST_SOURCE_FILE)
    get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
    string(REGEX REPLACE ".cpp" "_cpp_test" TEST_NAME ${TEST_FILENAME})
    message(STATUS  "adding ${TEST_NAME} ")
    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
    target_link_libraries(${TEST_NAME} co_sim_io)
    install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_cpp")
endfunction()

file(GLOB cpp_test_files "integration_tutorials/cpp/*.cpp")

foreach(cpp_test_file ${cpp_test_files})
add_cpp_executable(${cpp_test_file})
endforeach(cpp_test_file)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/run.sh $1\ &\ $2\ &\nwait\ %1\ &&\ wait\ %2)

# checking if the "run.sh" script return the correct exit codes
add_cpp_executable(test_success.cpp)
add_cpp_executable(test_fail.cpp)
add_test(NAME test_fail_fail       COMMAND sh run.sh $<TARGET_FILE:test_fail_cpp_test>  $<TARGET_FILE:test_fail_cpp_test>)
add_test(NAME test_success_fail    COMMAND sh run.sh $<TARGET_FILE:test_success_cpp_test>  $<TARGET_FILE:test_fail_cpp_test>)
add_test(NAME test_fail_success    COMMAND sh run.sh $<TARGET_FILE:test_fail_cpp_test>  $<TARGET_FILE:test_success_cpp_test>)
add_test(NAME test_success_success COMMAND sh run.sh $<TARGET_FILE:test_success_cpp_test>  $<TARGET_FILE:test_success_cpp_test>)
set_tests_properties(test_fail_fail test_success_fail test_fail_success PROPERTIES WILL_FAIL True)

add_test(hello_cpp_test hello_cpp_test)
add_test(NAME connect_disconnect_cpp_test COMMAND sh run.sh $<TARGET_FILE:connect_disconnect_a_cpp_test>  $<TARGET_FILE:connect_disconnect_b_cpp_test>)
add_test(NAME import_export_info_cpp_test COMMAND sh run.sh $<TARGET_FILE:export_info_cpp_test>  $<TARGET_FILE:import_info_cpp_test>)
add_test(NAME import_export_data_cpp_test COMMAND sh run.sh $<TARGET_FILE:export_data_cpp_test>  $<TARGET_FILE:import_data_cpp_test>)
add_test(NAME import_export_mesh_cpp_test COMMAND sh run.sh $<TARGET_FILE:export_mesh_cpp_test>  $<TARGET_FILE:import_mesh_cpp_test>)
add_test(NAME run_runner_cpp_test COMMAND sh run.sh $<TARGET_FILE:run_cpp_test>  $<TARGET_FILE:runner_cpp_test>)

if (CO_SIM_IO_ENABLE_MPI)
    function(add_cpp_mpi_executable TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE ".cpp" "_cpp_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_mpi ${MPI_LIBRARIES})
        install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_cpp_mpi")
    endfunction()

    file(GLOB cpp_mpi_test_files "integration_tutorials/cpp/mpi/*.cpp")

    foreach(cpp_mpi_test_file ${cpp_mpi_test_files})
    add_cpp_mpi_executable(${cpp_mpi_test_file})
    endforeach(cpp_mpi_test_file)
endif()

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

    file(GLOB c_test_files
        "co_sim_io/c/info/*.c"
        "co_sim_io/c/model_part/*.c"
    )

    foreach(c_test_file ${c_test_files})
    add_c_test(${c_test_file})
    endforeach(c_test_file)

    file(GLOB c_integration_test_files "integration_tutorials/c/*.c")

    foreach(c_integration_test_file ${c_integration_test_files})
    add_c_executable(${c_integration_test_file})
    endforeach(c_integration_test_file)

    add_test(hello_c_test hello_c_test)
    add_test(NAME connect_disconnect_c_test COMMAND bash run.sh $<TARGET_FILE:connect_disconnect_a_c_test>  $<TARGET_FILE:connect_disconnect_b_c_test>)
    add_test(NAME import_export_info_c_test COMMAND bash run.sh $<TARGET_FILE:export_info_c_test>  $<TARGET_FILE:import_info_c_test>)
    add_test(NAME import_export_data_c_test COMMAND bash run.sh $<TARGET_FILE:export_data_c_test>  $<TARGET_FILE:import_data_c_test>)
    add_test(NAME import_export_mesh_c_test COMMAND bash run.sh $<TARGET_FILE:export_mesh_c_test>  $<TARGET_FILE:import_mesh_c_test>)

    if (CO_SIM_IO_ENABLE_MPI)
        function(add_c_mpi_test TEST_SOURCE_FILE)
            get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
            string(REGEX REPLACE "[.]c" "_c_mpi_test" TEST_NAME ${TEST_FILENAME})
            message(STATUS  "adding ${TEST_NAME} ")
            add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
            target_link_libraries(${TEST_NAME} co_sim_io_c ${MPI_LIBRARIES})
            add_test(NAME ${TEST_NAME} COMMAND mpiexec -np 4 ${TEST_NAME})
            install(TARGETS ${TEST_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/tests_c_mpi")
        endfunction()

        file(GLOB c_mpi_test_files "co_sim_io/c/mpi/*.c")

        foreach(c_mpi_test_file ${c_mpi_test_files})
        add_c_mpi_test(${c_mpi_test_file})
        endforeach(c_mpi_test_file)

    endif()
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
