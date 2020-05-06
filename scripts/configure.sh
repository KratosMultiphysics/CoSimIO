# should be called with "sh scripts/configure.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_PYTHON=ON -DBUILD_FORTRAN=ON
cmake --build "build" --target install
