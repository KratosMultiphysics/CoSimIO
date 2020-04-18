rm -r build/
rm -r install/

cmake -H"." -B"build"
cmake --build "build" --target install
