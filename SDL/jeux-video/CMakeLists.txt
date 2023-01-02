cmake_minimum_required(VERSION 3.17)
set(glm_DIR /usr/lib/cmake/glm) 
find_package(glm REQUIRED)

message(STATUS "GLM includes ${GLM_INCLUDE_DIRS}")
message(STATUS "GLM libraries ${GLM_LIBRARY_DIRS}")

message("Hello")

project(draw2)

add_executable(${PROJECT_NAME} glad.c main.cpp)
target_link_libraries(${PROJECT_NAME} glm::glm GL dl glfw pthread)