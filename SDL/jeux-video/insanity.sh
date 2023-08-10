#!/bin/bash

rm -rf CMakeCache.txt CMakeFiles/ cmake_install.cmake txt.txt
# cmake . --debug-find &> txt.txt
# kate txt.txt
cmake .
make
./draw2
