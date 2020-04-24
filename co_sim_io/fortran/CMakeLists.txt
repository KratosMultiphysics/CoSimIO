project(CoSimIO_Fortran Fortran)
cmake_minimum_required (VERSION 2.8.6)

message("Configuring CoSimIO for Fortran")

if(${MSVC})
    # required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

add_library (co_sim_io_fortran SHARED ${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io.f90)

# # enable C++11
# set_target_properties(co_sim_io_c PROPERTIES
#     CXX_STANDARD 11
#     CXX_STANDARD_REQUIRED YES
#     CXX_EXTENSIONS NO
# )

install(TARGETS co_sim_io_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin)


# project( SimpleSolverFortran Fortran )

# include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../../co_sim_io/fortran )
# link_directories(${CMAKE_SOURCE_DIR}/install)

# add_executable (simple_solver_fortran ${CMAKE_CURRENT_SOURCE_DIR}/simple_solver.f90)
# target_link_libraries(simple_solver_fortran co_sim_io_c)

# install(TARGETS simple_solver_fortran DESTINATION ${CMAKE_SOURCE_DIR}/bin)












# cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# project(MyProject)

# enable_language(Fortran)

# # this is where we will place the Fortran module files
# set(CMAKE_Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/modules)

# # the executable is built from 3 source files
# add_executable(
#     myproject.x
#     main.F90
#     foo.F90
#     bar.F90
#     )