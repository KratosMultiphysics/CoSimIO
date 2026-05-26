~~~
   ______     _____ _           ________
  / ____/___ / ___/(_)___ ___  /  _/ __ |
 / /   / __ \\__ \/ / __ `__ \ / // / / /
/ /___/ /_/ /__/ / / / / / / // // /_/ /
\____/\____/____/_/_/ /_/ /_/___/\____/
Kratos CoSimulationApplication
~~~

<!--- [![CI](https://github.com/KratosMultiphysics/CoSimIO/actions/workflows/ci.yml/badge.svg?event=schedule)](https://github.com/KratosMultiphysics/CoSimIO/actions/workflows/ci.yml)-->
# CoSimIO

The _CoSimIO_ is a small library for interprocess communication in CoSimulation contexts. It is designed for exchanging data between different solvers or other software-tools. For performing coupled simulations it is used in combination with the [_CoSimulationApplication_](https://github.com/KratosMultiphysics/Kratos/tree/master/applications/CoSimulationApplication).\
It is implemented as a _detached interface_. This means that it follows the interface of Kratos but is independent of Kratos, which allows for an easy integration into other codes / solvers

The implementation is defined in [co_sim_io](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io)

Documentation and tutorials can be found [here](https://kratosmultiphysics.github.io/CoSimIO/)

Different methods for interprocess communication are available, their documentation can be found [here](https://kratosmultiphysics.github.io/CoSimIO/communication)

The tests are contained in [tests](https://github.com/KratosMultiphysics/CoSimIO/blob/master/tests)

The [changelog](https://github.com/KratosMultiphysics/CoSimIO/blob/master/CHANGELOG.md) contains information about relevant changes in the different versions.

## Dependencies
- The _CoSimIO_ is implemented as a standalone tool in C++. The only dependency is C++11. This includes most of the major C++ compilers in Windows, Linux, and Mac. A detailed list of C++11 compatible compilers [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11).
- Dependency on Kratos: There is **NO** dependency on Kratos. The _CoSimIO_ can be used completely without including or linking against Kratos.

## Available languages
Besides the native C++ interface, the _CoSimIO_ also provides interfaces to other languages. Currently the following languages are supported:
- C
- Python
- Fortran

These interfaces are implemented as consistent as possible with the C++ interface.

## Usage
This section provides a brief overview on how to use the _CoSimIO_. For more detailed explanations please check the [documentation](https://kratosmultiphysics.github.io/CoSimIO/) in the corresponding language.

### **C++**
The C++ interface is defined in [co_sim_io.hpp](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/co_sim_io.hpp). In addition to including this header it is required to compile _CoSimIO_ into a shared library and link against it.

### **C**
The C interface is defined in [co_sim_io_c.h](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/c/co_sim_io_c.h). In addition to including this header it is required to compile _CoSimIO_ into a shared library and link against it.

### **Python**
The Python interface is defined in [_CoSimIO_ python module](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/python/co_sim_io_python.cpp). The [pybind library](https://github.com/pybind/pybind11) is used for the Python exposure of the C++ interface.

### **Fortran**
The Fortran interface is defined in [co_sim_io.f90](https://github.com/KratosMultiphysics/CoSimIO/blob/master/co_sim_io/fortran/co_sim_io.f90). In addition to including this file it is required to compile _CoSimIO_ into a shared library and link against it.

## Parallelism
The _CoSimIO_ supports pure sequential and mpi-parallel executions. Shared memory parallelism is currently not planned but might be added at a later stage.

## Memory Management
Coupling requires frequent exchange of data. Therefore the _CoSimIO_ uses the memory provided by the including solvers/software-tools directly, without copying it to an auxiliar data structure.

## Installation

### Manual Installation

If you prefer not to use Spack, _CoSimIO_ can be built and installed manually using [CMake](https://cmake.org/).

#### Prerequisites

- A C++11 compatible compiler (GCC, Clang, MSVC, etc.) — see the full list [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)
- [CMake](https://cmake.org/) ≥ 3.15
- _(Optional)_ An MPI implementation (e.g. [OpenMPI](https://www.open-mpi.org/), [MPICH](https://www.mpich.org/)) for `+mpi` support
- _(Optional)_ [Python](https://www.python.org/) ≥ 3.6 and [pybind11](https://github.com/pybind/pybind11) for Python bindings
- _(Optional)_ A Fortran compiler for the Fortran API

---

#### 1. Clone the Repository

```bash
git clone https://github.com/KratosMultiphysics/CoSimIO.git
cd CoSimIO
```

Or download a specific release tarball:

```bash
wget https://github.com/KratosMultiphysics/CoSimIO/archive/refs/tags/v4.3.1.tar.gz
tar -xzf v4.3.1.tar.gz
cd CoSimIO-4.3.1
```

---

#### 2. Configure with CMake

Create a build directory and run CMake:

```bash
mkdir build && cd build
cmake ..
```

The following CMake options are available to control what gets built:

| CMake Option | Default | Description |
|---|---|---|
| `CO_SIM_IO_BUILD_C` | `ON` | Build the C API |
| `CO_SIM_IO_BUILD_PYTHON` | `OFF` | Build Python bindings |
| `CO_SIM_IO_BUILD_FORTRAN` | `OFF` | Build the Fortran API |
| `CO_SIM_IO_BUILD_TESTING` | `OFF` | Build the test suite |
| `CO_SIM_IO_BUILD_MPI` | `OFF` | Enable MPI communication |
| `CO_SIM_IO_STRICT_COMPILER` | `OFF` | Enable strict compiler warnings |

For example, to enable MPI and Python bindings:

```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCO_SIM_IO_BUILD_MPI=ON \
    -DCO_SIM_IO_BUILD_PYTHON=ON
```

To specify a custom install prefix:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
```

> **Note:** The Fortran API depends on the C API. Always enable `CO_SIM_IO_BUILD_C=ON` when building with `CO_SIM_IO_BUILD_FORTRAN=ON`.

---

#### 3. Build

```bash
cmake --build . --config Release
```

To speed up compilation using multiple cores:

```bash
cmake --build . --config Release -- -j$(nproc)
```

> On macOS, replace `$(nproc)` with `$(sysctl -n hw.logicalcpu)`.

---

#### 4. Install

```bash
cmake --install .
```

If a custom prefix was specified during configuration, the libraries and headers will be placed there. Otherwise the default system prefix (e.g. `/usr/local`) is used.

---

#### 5. Verify the Installation

After installation, confirm that the shared library is present:

```bash
ls /path/to/install/lib/libCoSimIO*
```

If Python bindings were built, verify the module is importable:

```bash
python3 -c "import CoSimIO; print('CoSimIO imported successfully')"
```

---

#### Platform-Specific Notes

| Platform | Notes |
|---|---|
| **Linux** | Ensure `/path/to/install/lib` is on `LD_LIBRARY_PATH` |
| **macOS** | Ensure `/path/to/install/lib` is on `DYLD_LIBRARY_PATH` |
| **Windows** | Ensure `/path/to/install/bin` is on `PATH`; use the Visual Studio generator with `-G "Visual Studio 17 2022"` |

### Installing with Spack

This section describes how to install _CoSimIO_ using [Spack](https://spack.io/), a flexible package manager designed for HPC environments. First, make sure Spack is available in your environment:

```bash
git clone --depth=1 https://github.com/spack/spack.git
source spack/share/spack/setup-env.sh
```

Then install _CoSimIO_ with the default configuration:

```bash
spack install cosimio
```

---

### Variants

The Spack package exposes several build variants that allow you to tailor the installation to your needs:

| Variant | Default | Description |
|---|---|---|
| `+mpi` / `~mpi` | `~mpi` | Enable MPI-parallel communication |
| `+c` / `~c` | `+c` | Build the C API |
| `+python` / `~python` | `~python` | Build Python bindings (requires Python ≥ 3.6) |
| `+fortran` / `~fortran` | `~fortran` | Build the Fortran API (requires `+c`) |
| `+testing` / `~testing` | `~testing` | Build the test suite |
| `+strict` / `~strict` | `~strict` | Enable strict compiler warnings |

> **Note:** The Fortran API is built on top of the C API. Enabling `+fortran` without `+c` is not allowed.

#### Examples

Install with MPI support:

```bash
spack install cosimio+mpi
```

Install with Python bindings:

```bash
spack install cosimio+python
```

Install with C, Fortran, and MPI support:

```bash
spack install cosimio+c+fortran+mpi
```

Install a specific version:

```bash
spack install cosimio@4.3.1
```

---

### Loading the Package

Once installed, load _CoSimIO_ into your environment with:

```bash
spack load cosimio
```

---

### Dependencies

Spack resolves all dependencies automatically. For reference, the following are pulled in depending on the chosen variants:

| Dependency | When required |
|---|---|
| `cmake@3.15:` | Always (build only) |
| `mpi` | `+mpi` |
| `python@3.6:` | `+python` |
| `py-pybind11` | `+python` (build only) |
| Fortran compiler | `+fortran` |

## Special Thanks To
- [Boost](http://www.boost.org/) for the `intrusive_ptr`
- [pybind11](https://github.com/pybind/pybind11) for exposing C++ to python
- [filesystem](https://github.com/gulrak/filesystem) Header-only single-file std::filesystem compatible helper library, based on the C++17 specs
- [asio](https://think-async.com/Asio/) for socket based interprocess communication
- [doctest](https://github.com/onqtam/doctest) C++ testing framework for the unit tests
