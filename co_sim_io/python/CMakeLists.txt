message("Configuring CoSimIO for Python")

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

pybind11_add_module(CoSimIO co_sim_io_python.cpp)

target_link_libraries( CoSimIO PRIVATE co_sim_io )

install(TARGETS CoSimIO DESTINATION ${CO_SIM_IO_INSTALL_DIRECTORY})
