message("Configuring CoSimIO for Python")

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

pybind11_add_module(CoSimIO co_sim_io_python.cpp)

target_link_libraries( CoSimIO PRIVATE co_sim_io )

install(TARGETS CoSimIO DESTINATION ${CMAKE_SOURCE_DIR}/bin)

if (CO_SIM_IO_ENABLE_MPI)
    pybind11_add_module(CoSimIOMPI co_sim_io_python_mpi.cpp)

    target_link_libraries( CoSimIOMPI PRIVATE co_sim_io co_sim_io_mpi )

    install(TARGETS CoSimIOMPI DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endif()