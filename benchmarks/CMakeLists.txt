include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io )

# CoSimIO benchmark sources. Disabled by default
if(${CO_SIM_IO_BUILD_BENCHMARK} MATCHES ON)
    file(GLOB_RECURSE CO_SIM_IO_BENCHMARK_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

    foreach(file ${CO_SIM_IO_BENCHMARK_SOURCES})
        get_filename_component(filename ${file} NAME_WE)
        add_executable(${filename} ${file})
        target_link_libraries(${filename} PUBLIC co_sim_io benchmark::benchmark)
        set_target_properties(${filename} PROPERTIES COMPILE_DEFINITIONS "CO_SIM_IO_BENCHMARK=IMPORT,API")
        install(TARGETS ${filename} DESTINATION "bin/benchmarks")
    endforeach(file ${CO_SIM_IO_BENCHMARK_SOURCES})
endif(${CO_SIM_IO_BUILD_BENCHMARK} MATCHES ON)

# TODO: Add MPI support in the future if required