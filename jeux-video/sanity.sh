#!/bin/bash

# i really hate how cmake was setup
rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

