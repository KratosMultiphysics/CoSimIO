# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

# export CXX=icpc
# export CC=icc

export CXX=clang++-10
export CC=clang-10

cmake -H"." -B"build" \
    -DBUILD_C=ON \
    -DBUILD_PYTHON=ON \
    -DCO_SIM_IO_ENABLE_MPI=ON \
    -DCMAKE_BUILD_TYPE=DEBUG \
    -DCO_SIM_IO_STRICT_COMPILER=ON
cmake --build "build" --target install -- -j5
