message("Configuring CoSimIO for Python")

# TODO make this configureable
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external_libraries/pybind11 dummy)

# Windows has to work in mysterious ways.
# From Windows' perspective, a shared library is a runtime binary
# bot not a module, even though it's pretty much a shared library
# as well.
function(install_pybind_module target)
    if (WIN32)
        install(TARGETS ${target}
            LIBRARY DESTINATION bin
            ARCHIVE DESTINATION bin
            RUNTIME DESTINATION bin)
    else()
        install(TARGETS ${target}
            LIBRARY DESTINATION libs
            ARCHIVE DESTINATION libs
            RUNTIME DESTINATION bin)
    endif()
endfunction()

pybind11_add_module(PyCoSimIO co_sim_io_python.cpp)

target_link_libraries( PyCoSimIO PRIVATE co_sim_io )

if (WIN32)
    set(CoSimIOPythonModuleRelativeDir "bin/CoSimIO")
else()
    set(CoSimIOPythonModuleRelativeDir "libs/CoSimIO")
endif()
set(CoSimIOPythonModuleDir "${CMAKE_INSTALL_PREFIX}/${CoSimIOPythonModuleRelativeDir}")

install_pybind_module(PyCoSimIO)
set_target_properties(PyCoSimIO PROPERTIES INSTALL_RPATH "$ORIGIN")

set(CoSimIOInitFile "${CoSimIOPythonModuleDir}/__init__.py")
set(CoSimIOMPIInitFile "${CoSimIOPythonModuleDir}/mpi/__init__.py")
set(mpi4pyInterfaceInitFile "${CoSimIOPythonModuleDir}/mpi/mpi4pyInterface/__init__.py")

file(WRITE "${CoSimIOInitFile}" "from PyCoSimIO import *\nfrom PyCoSimIO import __version__\n")

# dummy init files that give proper errors in case something related to MPI was not compiled
# These files will be overwritten if the corresponding option is enabled, hence here we can write then unconditionally
file(WRITE "${CoSimIOMPIInitFile}" "raise Exception('CoSimIO was compiled without MPI support! (use \"CO_SIM_IO_BUILD_MPI\" to enable it)')\n")
file(WRITE "${mpi4pyInterfaceInitFile}" "raise Exception('The mpi4py interface was not compiled! (use \"CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE\" to enable it)')\n")

if (CO_SIM_IO_BUILD_MPI)
    pybind11_add_module(PyCoSimIOMPI co_sim_io_python_mpi.cpp)

    target_link_libraries( PyCoSimIOMPI PRIVATE co_sim_io co_sim_io_mpi )

    install_pybind_module(PyCoSimIOMPI)
    set_target_properties(PyCoSimIOMPI PROPERTIES INSTALL_RPATH "$ORIGIN")
    file(WRITE "${CoSimIOMPIInitFile}" "from PyCoSimIOMPI import *\n")

    OPTION ( CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE "Building the interface to mpi4py MPI communicators" OFF )
    if (CO_SIM_IO_BUILD_PYTHON_MPI4PY_INTERFACE)
        message("Building the interface to mpi4py MPI communicators")
        pybind11_add_module(PyCoSimIOMPI_mpi4pyInterface mpi4py_interface.cpp)
        target_link_libraries( PyCoSimIOMPI_mpi4pyInterface PRIVATE ${MPI_LIBRARIES} )
        install_pybind_module(PyCoSimIOMPI_mpi4pyInterface)
        set_target_properties(PyCoSimIOMPI_mpi4pyInterface PROPERTIES INSTALL_RPATH "$ORIGIN")
        file(WRITE "${mpi4pyInterfaceInitFile}" "from PyCoSimIOMPI_mpi4pyInterface import *\n")
    endif()
endif()
