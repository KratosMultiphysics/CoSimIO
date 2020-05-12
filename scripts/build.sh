# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_PYTHON=ON -DBUILD_FORTRAN=OFF
cmake --build "build" --target install

