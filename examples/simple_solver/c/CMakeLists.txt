project( SimpleSolverC C)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../co_sim_io/c )
link_directories( ${CMAKE_SOURCE_DIR}/bin )

add_executable (simple_solver_c simple_solver.c)
target_link_libraries(simple_solver_c co_sim_io_c)

install(TARGETS simple_solver_c DESTINATION ${CMAKE_SOURCE_DIR}/bin )
