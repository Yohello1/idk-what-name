cmake_minimum_required(VERSION 3.0)
project(draw2)

if ( UNIX )
  set( CMAKE_CXX_FLAGS_DEBUG_INIT "-fstack-protector" )
elseif( WIN32 )
  set( CMAKE_CXX_FLAGS_DEBUG_INIT "/GS" )
endif()

SET(CMAKE_CXX_FLAGS "-g")

# set(glm_DIR /usr/lib/cmake/glm) 
# find_package(glm REQUIRED)
include_directories("/usr/include")
set(INCLUDE_DIRECTORIES /usr/include/)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)


message(STATUS "GLM includes ${GLM_INCLUDE_DIRS}")
message(STATUS "OpenGL includes ${OpenGL_INCLUDE_DIRS}")
message(STATUS "${OPENGL_gl_LIBRARY}")



add_executable(${PROJECT_NAME} glad.c main.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glm::glm dl glfw pthread GLU)
