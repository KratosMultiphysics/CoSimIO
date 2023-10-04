@echo off
rem Please do not modify this script. Copy it into the build folder

rem Set compiler
set CC=cl.exe
set CXX=cl.exe

rem Set variables
if not defined CMAKE_GENERATOR set CMAKE_GENERATOR=Visual Studio 16 2019
if not defined NUMBER_OF_COMPILATION_CORES set NUMBER_OF_COMPILATION_CORES=%NUMBER_OF_PROCESSORS%
if not defined COSIMIO_SOURCE set COSIMIO_SOURCE=%~dp0..
if not defined COSIMIO_BUILD set COSIMIO_BUILD=%COSIMIO_SOURCE%/build

rem Set defaults
if not defined CO_SIM_IO_BUILD_TESTING set CO_SIM_IO_BUILD_TESTING=ON
if not defined CO_SIM_IO_BUILD_C set CO_SIM_IO_BUILD_C=OFF
if not defined CO_SIM_IO_BUILD_PYTHON set CO_SIM_IO_BUILD_PYTHON=OFF
if not defined CO_SIM_IO_BUILD_FORTRAN set CO_SIM_IO_BUILD_FORTRAN=OFF
if not defined CO_SIM_IO_STRICT_COMPILER set CO_SIM_IO_STRICT_COMPILER=ON

rem Warning: In windows this option only works if you run through a terminal with admin privileges
rem set COSIMIO_INSTALL_PYTHON_USING_LINKS=ON

rem Set basic configuration
if not defined COSIMIO_BUILD_TYPE set COSIMIO_BUILD_TYPE=Release

rem Clean
del /F /Q "%COSIMIO_BUILD%\%COSIMIO_BUILD_TYPE%\cmake_install.cmake"
del /F /Q "%COSIMIO_BUILD%\%COSIMIO_BUILD_TYPE%\CMakeCache.txt"
del /F /Q "%COSIMIO_BUILD%\%COSIMIO_BUILD_TYPE%\CMakeFiles"

rem Configure
@echo on
cmake                                                   ^
-G "%CMAKE_GENERATOR%"                                  ^
-H"%COSIMIO_SOURCE%"                                    ^
-B"%COSIMIO_BUILD%\%COSIMIO_BUILD_TYPE%"                ^
-DCMAKE_BUILD_TYPE=%COSIMIO_BUILD_TYPE%                 ^
-DCO_SIM_IO_BUILD_MPI=OFF                               ^
-DCO_SIM_IO_BUILD_TESTING=%CO_SIM_IO_BUILD_TESTING%     ^
-DCO_SIM_IO_BUILD_C=%CO_SIM_IO_BUILD_C%                 ^
-DCO_SIM_IO_BUILD_PYTHON=%CO_SIM_IO_BUILD_PYTHON%       ^
-DCO_SIM_IO_BUILD_FORTRAN=%CO_SIM_IO_BUILD_FORTRAN%     ^
-DCO_SIM_IO_STRICT_COMPILER=%CO_SIM_IO_STRICT_COMPILER%

rem Build
IF "%CMAKE_GENERATOR%"=="Ninja" (
    cmake --build "%COSIMIO_BUILD%/%COSIMIO_BUILD_TYPE%" --target install -- -j%NUMBER_OF_COMPILATION_CORES%
) ELSE (
    cmake --build "%COSIMIO_BUILD%/%COSIMIO_BUILD_TYPE%" --target install -- /property:configuration=%COSIMIO_BUILD_TYPE% /p:Platform=x64
)

goto:eof