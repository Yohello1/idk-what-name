cmake_minimum_required(VERSION 3.0)

set(glm_DIR /usr/lib/cmake/glm) 
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

message(STATUS "GLM includes ${GLM_INCLUDE_DIRS}")
# message(STATUS "OpenGL includes ${OpenGL_INCLUDE_DIRS}")
message(STATUS "GLUT includes ${OpenGL_INCLUDE_DIRS}")

# message("Hello")

project(draw2)

add_executable(${PROJECT_NAME} glad.c main.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glm::glm dl glfw pthread)