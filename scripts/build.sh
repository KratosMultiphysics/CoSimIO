# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

# export CXX=icpc
# export CC=icc

cmake -H"." -B"build" \
    -DBUILD_C=ON \
    -DBUILD_PYTHON=OFF \
    -DBUILD_FORTRAN=OFF \
    -DCOSIMIO_ENABLE_MPI=OFF \
    -DSTRICT_COMPILER=ON
cmake --build "build" --target install -- -j$(nproc)
