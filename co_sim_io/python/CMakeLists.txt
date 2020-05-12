message("Configuring CoSimIO for Python")

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

pybind11_add_module(CoSimIO co_sim_io_python.cpp)

install(TARGETS CoSimIO DESTINATION ${CMAKE_SOURCE_DIR}/bin)
