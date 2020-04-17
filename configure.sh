rm -r build/
rm -r install/

cmake -H"." -B"build" -DENABLE_FORTRAN_EXAMPLES=ON
cmake --build "build" --target install
