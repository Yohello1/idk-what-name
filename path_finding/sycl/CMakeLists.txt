cmake_minimum_required(VERSION 2.8)

project(syclWork Version 1.0 Languages icpx)

target_include_directories(SYCL
	PUBLIC ${/opt/intel/oneapi/2024.1}/include)

add(executable draw2 syclTemp.cpp)
