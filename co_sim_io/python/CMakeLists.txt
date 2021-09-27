message("Configuring CoSimIO for Python")

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

pybind11_add_module(CoSimIO co_sim_io_python.cpp)

target_link_libraries( CoSimIO PRIVATE co_sim_io )

install(TARGETS CoSimIO DESTINATION ${CMAKE_SOURCE_DIR}/bin)

if (CO_SIM_IO_ENABLE_MPI)
    OPTION ( CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE "Building the interface to mpi4py MPI communicators" OFF )
    if (CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE)
        add_definitions( -DCO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE )
    endif()

    pybind11_add_module(CoSimIOMPI co_sim_io_python_mpi.cpp)

    target_link_libraries( CoSimIOMPI PRIVATE co_sim_io co_sim_io_mpi )

    install(TARGETS CoSimIOMPI DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endif()
