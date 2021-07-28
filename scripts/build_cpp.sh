# should be called with "sh scripts/build_cpp.sh" from the root-directory

cmake -H"." -B"build" -DBUILD_TESTING=ON
cmake --build "build" --target install -- -j$(nproc)
