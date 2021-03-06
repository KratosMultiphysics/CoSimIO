project(CoSimIO CXX C)
cmake_minimum_required (VERSION 2.8.6)

# this has to be specified BEFORE including CTest!
# suppressions file has to be included in the options, as using "MEMORYCHECK_SUPPRESSIONS_FILE" doesn't work on all systems
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck --error-exitcode=1 --suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind_suppressions.supp --gen-suppressions=all")

include(CTest)

# TODO check how this can be added (supported from CMake 3.17)
# list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

OPTION ( CO_SIM_IO_ENABLE_MPI "Enable MPI communication and MPI examples" OFF )
OPTION ( BUILD_C            "Building the CoSimIO for C"                OFF )
OPTION ( BUILD_PYTHON       "Building the CoSimIO for Python"           OFF )
OPTION ( BUILD_FORTRAN      "Building the CoSimIO for Fortran"          OFF )
OPTION ( CO_SIM_IO_STRICT_COMPILER    "Compiler has more warnings"                OFF )

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message("Building the CoSimIO with the following configuration:")
message("    CMAKE_BUILD_TYPE:   " ${CMAKE_BUILD_TYPE})
message("    CO_SIM_IO_ENABLE_MPI: " ${CO_SIM_IO_ENABLE_MPI})
message("    BUILD_TESTING:      " ${BUILD_TESTING})
message("    BUILD_C:            " ${BUILD_C})
message("    BUILD_PYTHON:       " ${BUILD_PYTHON})
message("    BUILD_FORTRAN:      " ${BUILD_FORTRAN})
message("    CO_SIM_IO_STRICT_COMPILER:    " ${CO_SIM_IO_STRICT_COMPILER})
message("")

include_directories(SYSTEM "external_libraries/asio/include")

if (CO_SIM_IO_ENABLE_MPI)
    find_package(MPI REQUIRED)
    add_definitions( -DCO_SIM_IO_USING_MPI )
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
endif()

if(MSVC)
    # /Zc:__cplusplus: required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    # /wd4251: "... needs to have dll-interface to be used by clients of class ..."
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /EHsc /wd4251")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    endif()

elseif(${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Werror")
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0 AND CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-override -Wignored-qualifiers")
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Werror")
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror-all  -diag-disable=2196") # 2196 currently comes from doctest
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror-all")
    endif()

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89 -Wall -Wpedantic")
endif()

message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_C_FLAGS:" ${CMAKE_C_FLAGS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # to suppress warning "MACOSX_RPATH is not specified for the following targets"
    set(CMAKE_MACOSX_RPATH 0)
endif()

# clean the bin directory
file(REMOVE_RECURSE bin)

# Adding CoSimIO library
file(GLOB co_sim_io_source_files
    ${CMAKE_SOURCE_DIR}/co_sim_io/src/*.cpp
    ${CMAKE_SOURCE_DIR}/co_sim_io/src/communication/*.cpp
)
add_library (co_sim_io SHARED ${co_sim_io_source_files})

# To automatically configure a library export header into build tree
include(GenerateExportHeader)
generate_export_header( co_sim_io EXPORT_MACRO_NAME CO_SIM_IO_API EXPORT_FILE_NAME
                        ${CMAKE_SOURCE_DIR}/co_sim_io/impl/co_sim_io_api.hpp )

install(TARGETS co_sim_io DESTINATION ${CMAKE_SOURCE_DIR}/bin)


if (CO_SIM_IO_ENABLE_MPI)
    # Adding CoSimIO library
    file(GLOB co_sim_io_mpi_source_files ${CMAKE_SOURCE_DIR}/co_sim_io/src/mpi/*.cpp)
    add_library (co_sim_io_mpi SHARED ${co_sim_io_mpi_source_files})

    target_link_libraries(co_sim_io_mpi co_sim_io)

    install(TARGETS co_sim_io_mpi DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endif()

if (BUILD_C)
    add_subdirectory(co_sim_io/c)
endif()

if (BUILD_PYTHON)
    add_subdirectory(co_sim_io/python)
endif()

if (BUILD_FORTRAN)
    # TODO since fortran relies on C, a check is needed
    message( FATAL_ERROR "The CoSimIO is currently not working for Fortran")
    add_subdirectory(co_sim_io/fortran)
endif()

if (BUILD_TESTING)
    if(CMAKE_MAJOR_VERSION LESS 3)
        message(FATAL_ERROR "Building the tests requires CMake 3")
    endif()
    include(external_libraries/doctest/doctest.cmake)
    add_subdirectory(tests)
endif()

configure_file(
  "tests/compiled_config.json.in"
  "${CMAKE_SOURCE_DIR}/tests/compiled_config.json"
)
