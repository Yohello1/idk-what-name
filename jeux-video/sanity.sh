#!/bin/bash

# i really hate how cmake was setup
rm -rf build
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
ninja test
ninja test_memcheck

