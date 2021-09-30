include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )


### Test runner for tests of CoSimIO
find_package (Threads REQUIRED)
file( GLOB CO_SIM_IO_TEST_FILES co_sim_io/cpp/*.cpp)
add_executable ( co_sim_io_tests ${CO_SIM_IO_TEST_FILES})
target_link_libraries(co_sim_io_tests co_sim_io ${CMAKE_THREAD_LIBS_INIT})
install( TARGETS co_sim_io_tests DESTINATION bin )
set_target_properties(co_sim_io_tests PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

doctest_discover_tests(co_sim_io_tests TEST_PREFIX "cpp_" WORKING_DIRECTORY $<TARGET_FILE_DIR:co_sim_io>)

if (CO_SIM_IO_BUILD_MPI)
    file( GLOB CO_SIM_IO_MPI_TEST_FILES co_sim_io/cpp/mpi/*.cpp )
    add_executable ( co_sim_io_mpi_tests ${CO_SIM_IO_MPI_TEST_FILES} )
    target_link_libraries( co_sim_io_mpi_tests co_sim_io_mpi)
    install( TARGETS co_sim_io_mpi_tests DESTINATION bin )
    set_target_properties(co_sim_io_mpi_tests PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    doctest_discover_tests(co_sim_io_mpi_tests TEST_PREFIX "cpp_mpi_" WORKING_DIRECTORY $<TARGET_FILE_DIR:co_sim_io_mpi> TEST_EXECUTOR mpiexec TEST_EXECUTOR_ARGS -np 4)

endif()

### C++ tests ###
function(add_cpp_executable TEST_SOURCE_FILE)
    get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
    string(REGEX REPLACE ".cpp" "_cpp_test" TEST_NAME ${TEST_FILENAME})
    message(STATUS  "adding ${TEST_NAME} ")
    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
    target_link_libraries(${TEST_NAME} co_sim_io)
    install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_cpp")
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

function(add_mpi_test test_name exe_1 exe_2)
    foreach(num_processes RANGE 2 4)
        set(full_test_name ${test_name}_mpi_test_${num_processes})
        message(STATUS  "adding MPI test ${full_test_name}")
        add_test(NAME ${full_test_name} COMMAND sh run_mpi.sh ${exe_1} ${exe_2} ${num_processes})
    endforeach(num_processes)
endfunction()

if (CO_SIM_IO_BUILD_MPI)
    # helper script to run two executables with MPI
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/run_mpi.sh mpiexec\ -np\ $3\ $1\ &\ mpiexec\ -np\ $3\ $2\ &\nwait\ %1\ &&\ wait\ %2)

    function(add_cpp_mpi_executable TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE ".cpp" "_cpp_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_mpi)
        install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_cpp_mpi")
    endfunction()

    file(GLOB cpp_mpi_test_files "integration_tutorials/cpp/mpi/*.cpp")

    foreach(cpp_mpi_test_file ${cpp_mpi_test_files})
        add_cpp_mpi_executable(${cpp_mpi_test_file})
    endforeach(cpp_mpi_test_file)

    add_mpi_test(connect_disconnect_cpp $<TARGET_FILE:connect_disconnect_mpi_a_cpp_test> $<TARGET_FILE:connect_disconnect_mpi_b_cpp_test>)
    add_mpi_test(import_export_info_cpp $<TARGET_FILE:export_info_mpi_cpp_test> $<TARGET_FILE:import_info_mpi_cpp_test>)
    add_mpi_test(import_export_data_cpp $<TARGET_FILE:export_data_mpi_cpp_test> $<TARGET_FILE:import_data_mpi_cpp_test>)
    add_mpi_test(import_export_mesh_cpp $<TARGET_FILE:export_mesh_mpi_cpp_test> $<TARGET_FILE:import_mesh_mpi_cpp_test>)
endif()

### C tests ###
if(CO_SIM_IO_BUILD_C)
    function(add_c_test TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE "[.]c" "_c_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_c)
        add_test(${TEST_NAME} ${TEST_NAME})
        install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_c")
    endfunction()

    function(add_c_executable TEST_SOURCE_FILE)
        get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
        string(REGEX REPLACE "[.]c" "_c_test" TEST_NAME ${TEST_FILENAME})
        message(STATUS  "adding ${TEST_NAME} ")
        add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
        target_link_libraries(${TEST_NAME} co_sim_io_c)
        install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_c")
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
    add_test(NAME connect_disconnect_c_test COMMAND bash run.sh $<TARGET_FILE:connect_disconnect_a_c_test> $<TARGET_FILE:connect_disconnect_b_c_test>)
    add_test(NAME import_export_info_c_test COMMAND bash run.sh $<TARGET_FILE:export_info_c_test> $<TARGET_FILE:import_info_c_test>)
    add_test(NAME import_export_data_c_test COMMAND bash run.sh $<TARGET_FILE:export_data_c_test> $<TARGET_FILE:import_data_c_test>)
    add_test(NAME import_export_mesh_c_test COMMAND bash run.sh $<TARGET_FILE:export_mesh_c_test> $<TARGET_FILE:import_mesh_c_test>)

    if (CO_SIM_IO_BUILD_MPI)
        function(add_c_mpi_test_executable TEST_SOURCE_FILE)
            get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
            string(REGEX REPLACE "[.]c" "_c_test" TEST_NAME ${TEST_FILENAME})
            add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
            target_link_libraries(${TEST_NAME} co_sim_io_c_mpi)
            install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_c_mpi")
        endfunction()

        function(add_c_mpi_test TEST_SOURCE_FILE)
            get_filename_component(TEST_FILENAME ${TEST_SOURCE_FILE} NAME)
            string(REGEX REPLACE "[.]c" "_mpi_c_test" TEST_NAME ${TEST_FILENAME})
            message(STATUS  "adding ${TEST_NAME} ")
            add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})
            target_link_libraries(${TEST_NAME} co_sim_io_c_mpi)
            add_test(NAME ${TEST_NAME} COMMAND mpiexec -np 4 ${TEST_NAME})
            install(TARGETS ${TEST_NAME} DESTINATION "bin/tests_c_mpi")
        endfunction()

        file(GLOB c_mpi_test_files "co_sim_io/c/mpi/*.c")
        file(GLOB c_mpi_test_exec_files "integration_tutorials/c/mpi/*.c")

        foreach(c_mpi_test_file ${c_mpi_test_files})
            add_c_mpi_test(${c_mpi_test_file})
        endforeach(c_mpi_test_file)

        foreach(c_mpi_test_file ${c_mpi_test_exec_files})
            add_c_mpi_test_executable(${c_mpi_test_file})
        endforeach(c_mpi_test_file)

        add_mpi_test(connect_disconnect_c $<TARGET_FILE:connect_disconnect_mpi_a_c_test> $<TARGET_FILE:connect_disconnect_mpi_b_c_test>)
        add_mpi_test(import_export_info_c $<TARGET_FILE:export_info_mpi_c_test> $<TARGET_FILE:import_info_mpi_c_test>)
        add_mpi_test(import_export_data_c $<TARGET_FILE:export_data_mpi_c_test> $<TARGET_FILE:import_data_mpi_c_test>)
        add_mpi_test(import_export_mesh_c $<TARGET_FILE:export_mesh_mpi_c_test> $<TARGET_FILE:import_mesh_mpi_c_test>)

    endif()
endif()


### Fortran tests ###
if(CO_SIM_IO_BUILD_FORTRAN)
    enable_language( Fortran )
    include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
    link_directories( bin )

    # file( GLOB_RECURSE FORTRAN_TEST_FILES *.f90)
    # add_executable ( test_runner_fortran test_runners/test_runner.f90 ${FORTRAN_TEST_FILES})
    # target_link_libraries(test_runner_fortran co_sim_io_c)

    # install( TARGETS test_runner_fortran DESTINATION bin )
endif()
