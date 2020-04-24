rm -r build/
rm -r bin/

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_PYTHON=ON -DBUILD_FORTRAN=ON
cmake --build "build" --target install
