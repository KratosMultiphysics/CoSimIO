# should be called with "sh scripts/build_c.sh" from the root-directory

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_TESTING=ON
cmake --build "build" --target install -- -j$(nproc)
