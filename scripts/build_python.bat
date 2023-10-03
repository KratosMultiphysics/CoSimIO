@echo off
REM Should be copied to the build directory

set COSIMIO_SOURCE=%~dp0..
set CO_SIM_IO_BUILD_PYTHON=ON
call %COSIMIO_SOURCE%\scripts\build.bat