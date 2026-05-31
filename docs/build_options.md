# Build options for the _CoSimIO_

The CoSimIO uses [CMake](https://cmake.org/) as build system. The following options are available for configuration:

| name | default| description |
|---|---|---|
| CO_SIM_IO_BUILD_TYPE | `CMAKE_BUILD_TYPE` (in case it is defined), otherwise `Release` | Build type with which the _CoSimIO_ is built. Use `Release` for production and `Debug` to enable more runtime checks (at the cost of being slower).  |
| CMAKE_INSTALL_PREFIX | source directory of CoSimIO | Used to specify the install directory. Binaries will be installed in `${CMAKE_INSTALL_PREFIX}/bin` |
| CO_SIM_IO_BUILD_TESTING | ON (same as `BUILD_TESTING`) | Build the tests. Useful for developing and initial installation to check if everything works as expected. |
| CO_SIM_IO_BUILD_MPI | OFF | Enable MPI support for distributed environments. Requires an installation of [MPI](https://www.mpi-forum.org/). |
| CO_SIM_IO_BUILD_MPI_COMMUNICATION | OFF | Enable communication via MPI. Requires to build CoSimIO with MPI support (using `CO_SIM_IO_BUILD_MPI`) |
| CO_SIM_IO_BUILD_C | OFF | Build the C-interface |
| CO_SIM_IO_BUILD_PYTHON | OFF | Build the Python-interface |
| CO_SIM_IO_BUILD_FORTRAN | OFF | Build the Fortran-interface (requires the C-interface) |
| CO_SIM_IO_STRICT_COMPILER | OFF | Enable more warnings in the compiler, useful for development. |

---

The configuration that is used is also printed when compiling, e.g.

~~~sh
Building the CoSimIO with the following configuration:
    CO_SIM_IO_BUILD_TYPE:      Release
    CO_SIM_IO_BUILD_MPI:       ON
    CO_SIM_IO_BUILD_TESTING:   ON
    CO_SIM_IO_BUILD_C:         ON
    CO_SIM_IO_BUILD_PYTHON:    ON
    CO_SIM_IO_BUILD_FORTRAN:   ON
    CO_SIM_IO_STRICT_COMPILER: ON
~~~

## Installation

### Manual Installation

If you prefer not to use Spack, _CoSimIO_ can be built and installed manually using [CMake](https://cmake.org/).

#### Prerequisites

- A C++11 compatible compiler (GCC, Clang, MSVC, etc.) — see the full list [here](https://en.cppreference.com/w/cpp/compiler_support#cpp11)
- [CMake](https://cmake.org/) ≥ 3.15
- _(Optional)_ An MPI implementation (e.g. [OpenMPI](https://www.open-mpi.org/), [MPICH](https://www.mpich.org/)) for `+mpi` support
- _(Optional)_ [Python](https://www.python.org/) ≥ 3.6 and [pybind11](https://github.com/pybind/pybind11) for Python bindings
- _(Optional)_ A Fortran compiler for the Fortran API

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

#### 3. Build

```bash
cmake --build . --config Release
```

To speed up compilation using multiple cores:

```bash
cmake --build . --config Release -- -j$(nproc)
```

> On macOS, replace `$(nproc)` with `$(sysctl -n hw.logicalcpu)`.

#### 4. Install

```bash
cmake --install .
```

If a custom prefix was specified during configuration, the libraries and headers will be placed there. Otherwise the default system prefix (e.g. `/usr/local`) is used.

#### 5. Verify the Installation

After installation, confirm that the shared library is present:

```bash
ls /path/to/install/lib/libCoSimIO*
```

If Python bindings were built, verify the module is importable (defining properly the `PYTHONPATH`):

```bash
python3 -c "import CoSimIO; print('CoSimIO imported successfully')"
```

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

### Loading the Package

Once installed, load _CoSimIO_ into your environment with:

```bash
spack load cosimio
```

### Dependencies

Spack resolves all dependencies automatically. For reference, the following are pulled in depending on the chosen variants:

| Dependency | When required |
|---|---|
| `cmake@3.15:` | Always (build only) |
| `mpi` | `+mpi` |
| `python@3.6:` | `+python` |
| `py-pybind11` | `+python` (build only) |
| Fortran compiler | `+fortran` |
