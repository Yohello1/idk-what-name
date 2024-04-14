SHELL := /bin/bash

test : test.cpp
	acpp syclTemp.cpp

helloWorld : helloSycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl helloSycl.cpp

listDevices : listDevicesSycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl listDevicesSycl.cpp

dataCopySycl : dataCopySycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl dataCopySycl.cpp

cpuSelector : cpuSelectorSycl.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl cpuSelectorSycl.cpp

explicitMemory : explicitMemory.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl explicitMemory.cpp

implicitMemory : implicitMemory.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl implicitMemory.cpp

bufferAccessor : bufferAccessor.cpp
	icpx -fsycl -I /opt/intel/oneapi/2024.1/include -I /opt/intel/oneapi/2024.1/include/sycl bufferAccessor.cpp

gravity : gravity.cpp
	acpp  -lsfml-graphics -lsfml-window -lsfml-system gravity.cpp
