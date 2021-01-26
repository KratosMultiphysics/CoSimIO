# should be called with "sh scripts/build.sh" from the root-directory

rm -r build/

cmake -H"." -B"build" \
    -DBUILD_C=ON \
    -DBUILD_PYTHON=ON \
    -DBUILD_FORTRAN=OFF \
    -DENABLE_SOCKETS=OFF \
    -DENABLE_MPI=OFF \
    -DCMAKE_BUILD_TYPE=Debug
cmake --build "build" --target install -- -j$(nproc)

