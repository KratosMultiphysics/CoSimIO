message("Configuring CoSimIO for C")

add_library (co_sim_io_c SHARED co_sim_io_c.cpp)
set_target_properties(co_sim_io_c PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_link_libraries( co_sim_io_c co_sim_io )

install(TARGETS co_sim_io_c DESTINATION ${CMAKE_SOURCE_DIR}/bin)
