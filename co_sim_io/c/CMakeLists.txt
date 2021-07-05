message("Configuring CoSimIO for C")

# generate_export_header support for C project was added in CMake 3.12
# hence lets use this for now
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

add_library (co_sim_io_c SHARED co_sim_io_c.cpp)

target_link_libraries( co_sim_io_c co_sim_io )

install(TARGETS co_sim_io_c DESTINATION ${CMAKE_SOURCE_DIR}/bin)
