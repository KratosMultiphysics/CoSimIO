# should be called with "sh scripts/build.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_PYTHON=ON -DBUILD_FORTRAN=OFF -DCMAKE_BUILD_TYPE=Debug -DMEMORYCHECK_COMMAND_OPTIONS="--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1"
cmake --build "build" --target install

