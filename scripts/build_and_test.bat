@echo off
REM should be called with "scripts\build_and_test.bat" from the build directory

REM rmdir /s /q build
if not defined COSIMIO_BUILD_TYPE set COSIMIO_BUILD_TYPE=Release
set COSIMIO_SOURCE=%~dp0..
set CO_SIM_IO_BUILD_TESTING=ON
set CO_SIM_IO_BUILD_PYTHON=ON
call %COSIMIO_SOURCE%\scripts\build.bat

echo "Running CTests"
cd %COSIMIO_SOURCE%\build\%COSIMIO_BUILD_TYPE%
REM The following command may need to be adjusted depending on the exact behavior of "ctest" on Windows.
ctest --output-on-failure
if errorlevel 1 (
    echo CTests failed, but continuing...
)

echo "Running Python Tests"
set PATH=%PATH%;%COSIMIO_SOURCE%\bin
set PYTHONPATH=%PYTHONPATH%;%COSIMIO_SOURCE%\bin
py.exe %COSIMIO_SOURCE%\tests\run_python_tests.py
