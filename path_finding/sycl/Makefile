SHELL := /bin/bash

test : test.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl syclTemp.cpp

helloWorld : helloSycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl helloSycl.cpp

listDevices : listDevicesSycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl listDevicesSycl.cpp
