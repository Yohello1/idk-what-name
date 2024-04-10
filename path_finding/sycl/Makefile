SHELL := /bin/bash

test : test.cpp
	source /opt/intel/oneapi/setvars.sh && icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl syclTemp.cpp

helloWorld : helloSycl.cpp
	source /opt/intel/oneapi/setvars.sh && icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl helloSycl.cpp
