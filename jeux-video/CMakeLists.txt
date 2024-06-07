cmake_minimum_required(VERSION 3.1)
set(CMAKE_BUILD_TYPE Debug)

#add_compile_options(-g -Wall -Wextra -Wpedantic)
include_directories(src/)

project(posionedchlorine)
set(CMAKE_CXX_STANDARD 23)
include(CTest)

file(GLOB_RECURSE SOURCES src/*.cpp)

#SET(GCC_COVERAGE_COMPILE_FLAGS "-g")
#SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

add_executable(posionedchlorine ${SOURCES})
add_test(example_test posionedchlorine)

add_custom_target(test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND}
        --force-new-ctest-process --test-action memcheck
    COMMAND ${CMAKE_COMMAND} -E cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
