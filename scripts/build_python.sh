# should be called with "sh scripts/build_python.sh" from the root-directory

rm -r build/

cmake -H"." -B"build" -DBUILD_PYTHON=ON -DBUILD_TESTING=ON
cmake --build "build" --target install -- -j$(nproc)
