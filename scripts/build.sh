# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

# export CXX=icpc
# export CC=icc

cmake -H"." -B"build" \
    -DBUILD_C=ON \
    -DBUILD_PYTHON=ON \
    -DCO_SIM_IO_ENABLE_MPI=ON \
    -DSIM_IO_ENABLE_STRICT_COMPILER=ON
cmake --build "build" --target install -- -j$(nproc)
