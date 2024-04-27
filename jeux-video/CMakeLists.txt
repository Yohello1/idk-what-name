cmake_minimum_required(VERSION 3.1)

project(posionedchlorine)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(posionedchlorine ${SOURCES})
