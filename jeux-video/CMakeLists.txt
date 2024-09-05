cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)

project(pChlorine)

add_executable(pChlorine src/main.cpp
src/chlorine/io/io.hpp
src/chlorine/logs/logs.hpp
src/chlorine/scene/component.hpp
src/chlorine/scene/scene.hpp
src/chlorine/scene/componentManager.hpp
src/chlorine/utils/strings.hpp
src/chlorine/io/io.cpp
src/chlorine/logs/logs.cpp
src/chlorine/scene/scene.cpp
src/chlorine/scene/componentManager.cpp
src/chlorine/utils/strings.cpp
)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  endif()
endif()

target_include_directories(pChlorine PRIVATE src)
