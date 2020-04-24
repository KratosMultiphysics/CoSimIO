project( SimpleSolverFortran Fortran )

include_directories( ${CMAKE_SOURCE_DIR}/co_sim_io/fortran )
link_directories(${CMAKE_SOURCE_DIR}/bin)

add_executable( simple_solver_fortran simple_solver.f90 ${CMAKE_SOURCE_DIR}/co_sim_io/fortran/co_sim_io.f90)
target_link_libraries( simple_solver_fortran co_sim_io_c )

install( TARGETS simple_solver_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin )
