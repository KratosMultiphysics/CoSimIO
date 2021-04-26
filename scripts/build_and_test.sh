# !/bin/bash
set -e

# should be called with "sh scripts/build_and_test.sh" from the root-directory

# rm -r build/

sh scripts/build.sh

echo $'\n>>> Running CTests'
cd build
ctest -T memcheck --output-on-failure || true # don't stop if this fails

echo $'\n>>> Running Python Tests'
cd tests
python3 run_python_tests.py
