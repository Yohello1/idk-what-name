
# Note to move this over to Meson
# Let's contue using CMake, nvm

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(pChlorine)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

add_library(glad STATIC
    include/glad/glad.c
)
target_include_directories(glad PUBLIC include)


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


target_link_libraries(pChlorine PRIVATE
                                glfw
                                glm::glm
                                OpenGL::GL
                                glad
                                )


target_include_directories(pChlorine PRIVATE src)
