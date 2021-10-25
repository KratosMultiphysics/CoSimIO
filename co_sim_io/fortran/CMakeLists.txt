message("Configuring CoSimIO for Fortran")

enable_language(Fortran)


add_library (co_sim_io_fortran SHARED co_sim_io.f90)

target_link_libraries( co_sim_io_fortran co_sim_io_c )

install(TARGETS co_sim_io_fortran DESTINATION bin)

add_executable(fTest test.f90)
target_link_libraries( fTest co_sim_io_fortran )
