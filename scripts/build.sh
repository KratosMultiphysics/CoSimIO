# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" \
    -DBUILD_C=OFF \
    -DBUILD_PYTHON=ON \
    -DBUILD_FORTRAN=OFF \
    -DCMAKE_BUILD_TYPE=Debug
cmake --build "build" --target install

