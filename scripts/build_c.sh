# should be called with "sh scripts/build_c.sh" from the root-directory

# rm -r build/

cmake -H"." -B"build" -DBUILD_C=ON -DBUILD_TESTING=ON
cmake --build "build" --target install -- -j$(nproc)
