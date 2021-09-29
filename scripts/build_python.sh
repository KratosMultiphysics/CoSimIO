# should be called with "sh scripts/build_python.sh" from the root-directory

cmake -H"." -B"build" -DCO_SIM_IO_BUILD_PYTHON=ON -DBUILD_TESTING=ON -DCO_SIM_IO_ENABLE_MPI=OFF
cmake --build "build" --target install -- -j$(nproc)
