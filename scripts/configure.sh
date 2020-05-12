# should be called with "sh scripts/configure.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_C=OFF -DBUILD_PYTHON=ON -DBUILD_FORTRAN=OFF
cmake --build "build" --target install
