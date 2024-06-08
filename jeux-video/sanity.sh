#!/bin/bash

# i really hate how cmake was setup
rm -rf build
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
ninja test
ninja test_memcheck

echo "python
import sys
sys.path.insert(0, '/usr/share/gcc-8.2.1/python/')
# This would only enable the printers but not the xmethods:
# from libstdcxx.v6.printers import register_libstdcxx_printers
from libstdcxx.v6 import register_libstdcxx_printers
register_libstdcxx_printers (None)
end " > .gdbinit
