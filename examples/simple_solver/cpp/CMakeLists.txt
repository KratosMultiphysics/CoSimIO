project( SimpleSolverCpp CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../co_sim_io )

add_executable (simple_solver_cpp ${CMAKE_CURRENT_SOURCE_DIR}/simple_solver.cpp)

install(TARGETS simple_solver_cpp DESTINATION ${CMAKE_SOURCE_DIR}/install )