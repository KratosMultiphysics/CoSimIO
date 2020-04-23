rm -r build/
rm -r bin/

cmake -H"." -B"build"
cmake --build "build" --target install
