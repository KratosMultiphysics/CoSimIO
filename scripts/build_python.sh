# should be called with "sh scripts/configure.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_PYTHON=ON 
cmake --build "build" --target install
