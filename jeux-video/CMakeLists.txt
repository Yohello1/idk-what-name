cmake_minimum_required(VERSION 3.1)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-g -Wall -Wextra -Wpedantic)
include_directories(src/)

project(posionedchlorine)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_FLAGS_DEBUG  "-g")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

add_executable(posionedchlorine ${SOURCES})
