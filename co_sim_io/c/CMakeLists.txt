message("Configuring CoSimIO for C")

add_library (co_sim_io_c SHARED co_sim_io_c.cpp)

install(TARGETS co_sim_io_c DESTINATION ${CMAKE_SOURCE_DIR}/bin)
