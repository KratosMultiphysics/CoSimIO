# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

# export CXX=icpc
# export CC=icc

cmake -H"." -B"build" \
    -DBUILD_C=ON \
    -DBUILD_PYTHON=OFF \
    -DBUILD_FORTRAN=OFF \
    -DENABLE_SOCKETS=OFF \
    -DENABLE_MPI=ON \
    -DSTRICT_COMPILER=ON
cmake --build "build" --target install -- -j6
