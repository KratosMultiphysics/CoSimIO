cmake_minimum_required (VERSION 2.8.6)

if(CMAKE_MAJOR_VERSION LESS 3)
    project(CoSimIO CXX C)
else()
    if(POLICY CMP0048)
        # project command manages version
        cmake_policy(SET CMP0048 NEW)
    endif(POLICY CMP0048)
    project(CoSimIO LANGUAGES CXX C VERSION 3.0.0)
endif()

# this has to be specified BEFORE including CTest!
# suppressions file has to be included in the options, as using "MEMORYCHECK_SUPPRESSIONS_FILE" doesn't work on all systems
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck --error-exitcode=1 --suppressions=${CMAKE_CURRENT_SOURCE_DIR}/tests/valgrind_suppressions.supp --gen-suppressions=all")

include(CTest)

# TODO check how this can be added (supported from CMake 3.17)
# list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

OPTION ( CO_SIM_IO_BUILD_MPI          "Enable MPI communication and MPI examples" OFF )
OPTION ( CO_SIM_IO_BUILD_C            "Building the CoSimIO for C"                OFF )
OPTION ( CO_SIM_IO_BUILD_PYTHON       "Building the CoSimIO for Python"           OFF )
OPTION ( CO_SIM_IO_BUILD_FORTRAN      "Building the CoSimIO for Fortran"          OFF )
OPTION ( CO_SIM_IO_STRICT_COMPILER    "Compiler has more warnings"                OFF )
OPTION ( CO_SIM_IO_BUILD_TESTING      "Build tests"                               ${BUILD_TESTING} )
OPTION ( CO_SIM_IO_BUILD_BENCHMARK    "Build benchmarks"                          OFF )

if(NOT DEFINED CO_SIM_IO_BUILD_TYPE)
    if(CMAKE_BUILD_TYPE)
        set(CO_SIM_IO_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    else()
        set(CO_SIM_IO_BUILD_TYPE Release)
    endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCO_SIM_IO_DEBUG")

message("Building the CoSimIO with the following configuration:")
message("    CO_SIM_IO_BUILD_TYPE:      " ${CO_SIM_IO_BUILD_TYPE})
message("    CO_SIM_IO_BUILD_MPI:       " ${CO_SIM_IO_BUILD_MPI})
message("    CO_SIM_IO_BUILD_TESTING:   " ${CO_SIM_IO_BUILD_TESTING})
message("    CO_SIM_IO_BUILD_C:         " ${CO_SIM_IO_BUILD_C})
message("    CO_SIM_IO_BUILD_PYTHON:    " ${CO_SIM_IO_BUILD_PYTHON})
message("    CO_SIM_IO_BUILD_FORTRAN:   " ${CO_SIM_IO_BUILD_FORTRAN})
message("    CO_SIM_IO_STRICT_COMPILER: " ${CO_SIM_IO_STRICT_COMPILER})
message("    CO_SIM_IO_BUILD_BENCHMARK: " ${CO_SIM_IO_BUILD_BENCHMARK})
message("")

# Needs to be here as otherwise doesn't find MPI properly
if (CO_SIM_IO_BUILD_MPI)
    find_package(MPI REQUIRED)
    add_definitions( -DCO_SIM_IO_USING_MPI )
    set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
    set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
endif()

# Threads are needed for the CoSimIO library
find_package (Threads REQUIRED)

# detect whether CoSimIO is integrated into another project
get_directory_property(hasParent PARENT_DIRECTORY)

# Set installation directory
if (NOT hasParent)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        # Setting the cache path prevents it to change in case someone 'make' after modifying this file and not reconfiguring
        set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Default Install path" FORCE)
        message("-- Standard install dir ${CMAKE_INSTALL_PREFIX}")
    else(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        message("-- User defined install dir ${CMAKE_INSTALL_PREFIX}")
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(NOT hasParent)

# If the compiler is MSVC, we need to set the correct flags
if(MSVC)
    # /Zc:__cplusplus: required such that "__cplusplus" is set to the correct value
    # see https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    # Note: min value is c++14 => 201402L (c++11 does not exist, will also output 201402L)
    # /wd4251: "... needs to have dll-interface to be used by clients of class ..."
    # /wd4275: "non dll-interface class 'std::exception' used as base for dll-interface class 'CoSimIO::Internals::Exception'"
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /EHsc /wd4251 /wd4275")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    endif()

elseif(${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-parameter -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra -Werror")
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0 AND CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-override -Wignored-qualifiers")
    endif()

    # Always link with libstdc++fs.a when using GCC 8 (greater than 7 and less than 9)
    # Note: This command makes sure that this option comes pretty late on the cmdline.
    link_libraries("$<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,7.0>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:-lstdc++fs>")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wno-unused-parameter -Werror")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra -Werror")
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")

    if (CO_SIM_IO_STRICT_COMPILER)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror-all -diag-disable=2196") # 2196 currently comes from doctest
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror-all")
    endif()

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89 -Wall -Wpedantic")
endif()

message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_C_FLAGS:" ${CMAKE_C_FLAGS})

# In case of MacOS, we need to set CMP0042 to NEW to avoid warnings
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    cmake_policy(SET CMP0042 NEW)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Clean the bin directory
if (NOT hasParent)
    file(REMOVE_RECURSE bin)
endif(NOT hasParent)

# Adding CoSimIO library
file(GLOB_RECURSE co_sim_io_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/sources/*.cpp
)
add_library (co_sim_io SHARED ${co_sim_io_source_files})
target_link_libraries(co_sim_io ${CMAKE_THREAD_LIBS_INIT})

# To automatically configure a library export header into build tree
include(GenerateExportHeader)
generate_export_header( co_sim_io EXPORT_MACRO_NAME CO_SIM_IO_API EXPORT_FILE_NAME
                        ${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/includes/co_sim_io_api.hpp )

install(TARGETS co_sim_io DESTINATION bin)

# MPI communication
if (CO_SIM_IO_BUILD_MPI)
    # optionally enable communication via MPI
    OPTION ( CO_SIM_IO_BUILD_MPI_COMMUNICATION "Enabling communication via MPI" OFF )
    if (CO_SIM_IO_BUILD_MPI_COMMUNICATION)
        add_definitions( -DCO_SIM_IO_BUILD_MPI_COMMUNICATION )
        message("Enabled communication via MPI")
        if(MPI_CXX_VERSION_MAJOR VERSION_LESS 2.0)
            message(WARNING "Communication via MPI requires at least MPI version 2.0! Currently detected version: " ${MPI_CXX_VERSION})
        endif ()
    endif()

    # Adding CoSimIOMPI library
    file(GLOB_RECURSE co_sim_io_mpi_source_files ${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io/mpi/sources/*.cpp)
    add_library (co_sim_io_mpi SHARED ${co_sim_io_mpi_source_files})

    target_link_libraries(co_sim_io_mpi co_sim_io ${MPI_LIBRARIES})

    install(TARGETS co_sim_io_mpi DESTINATION bin)
endif(CO_SIM_IO_BUILD_MPI)

target_include_directories(co_sim_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/co_sim_io)
target_include_directories(co_sim_io SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external_libraries)
target_include_directories(co_sim_io SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external_libraries/asio/include)

# C bindings
if (CO_SIM_IO_BUILD_C)
    add_subdirectory(co_sim_io/c)
endif(CO_SIM_IO_BUILD_C)

# Python bindings
if (CO_SIM_IO_BUILD_PYTHON)
    add_subdirectory(co_sim_io/python)
endif(CO_SIM_IO_BUILD_PYTHON)

# Fortran bindings
if (CO_SIM_IO_BUILD_FORTRAN)
    if (NOT CO_SIM_IO_BUILD_C)
        message(FATAL_ERROR "Fortran requires the C interface!")
    endif()
    include(CMakeAddFortranSubdirectory)
    cmake_add_fortran_subdirectory(
        co_sim_io/fortran
        NO_EXTERNAL_INSTALL
    )
endif(CO_SIM_IO_BUILD_FORTRAN)

# Testing
if (CO_SIM_IO_BUILD_TESTING)
    if(CMAKE_MAJOR_VERSION LESS 3)
        message(FATAL_ERROR "Building the tests requires CMake 3")
    endif()
    include(external_libraries/doctest/doctest.cmake)
    add_subdirectory(tests)

    configure_file(
      "tests/compiled_config.json.in"
      "${CMAKE_CURRENT_SOURCE_DIR}/tests/compiled_config.json"
    )
endif(CO_SIM_IO_BUILD_TESTING)

# Benchmarking
if(CO_SIM_IO_BUILD_BENCHMARK MATCHES ON)
    # Includes for the Google Benchmark library
    # This function automatically configures a given application to build its benchmarks
    macro(co_sim_io_add_benchmarks)
        set(options USE_MPI USE_CUSTOM_MAIN)
        set(oneValueArgs TARGET WORKING_DIRECTORY)
        set(multiValueArgs SOURCES)

        cmake_parse_arguments(CO_SIM_IO_ADD_BENCHMARK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        if(CO_SIM_IO_ADD_BENCHMARK_SOURCES)
            include(GoogleBenchmark)
        endif(CO_SIM_IO_ADD_BENCHMARK_SOURCES)
    endmacro(co_sim_io_add_benchmarks)

    # Add the definitions if required
    ADD_DEFINITIONS(-DCO_SIM_IO_BUILD_BENCHMARKING)

    # Retrieve a copy of the current directory's `COMPILE_OPTIONS`
    get_directory_property(co_sim_io_root_compile_options COMPILE_OPTIONS)

    # Disable warnings (needed by centos. We should all love centos, it clearly needs some affection)
    add_compile_options(-w)

    # Fetch the Google Benchmark library
    include(FetchContent)
    FetchContent_Declare(
        googlebenchmark
        URL https://github.com/google/benchmark/archive/v1.9.0.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(CMAKE_INSTALL_LIBDIR "bin")
    set(CMAKE_INSTALL_BINDIR "bin")
    FetchContent_GetProperties(googlebenchmark)
    if(NOT googlebenchmark_POPULATED)
      FetchContent_Populate(googlebenchmark)
      add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif(NOT googlebenchmark_POPULATED)

    # Restore the current directory's old `COMPILE_OPTIONS`
    set_directory_properties(PROPERTIES COMPILE_OPTIONS "${co_sim_io_root_compile_options}")

    # Add the benchmarking directory
    add_subdirectory(benchmarks)
endif(CO_SIM_IO_BUILD_BENCHMARK MATCHES ON)