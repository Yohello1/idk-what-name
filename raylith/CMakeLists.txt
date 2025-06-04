cmake_minimum_required(VERSION 3.10)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Source file
set(SRC src/main.cpp)

# Find Vulkan and GLFW
find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Output directory for SPIR-V
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SPIRV_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY ${SPIRV_DIR})

# Compile shaders with glslc
add_custom_command(
    OUTPUT ${SPIRV_DIR}/vert.spv
    COMMAND glslc ${SHADER_DIR}/shader.vert -o ${SPIRV_DIR}/vert.spv
    DEPENDS ${SHADER_DIR}/shader.vert
    COMMENT "Compiling shader.vert to vert.spv"
)

add_custom_command(
    OUTPUT ${SPIRV_DIR}/frag.spv
    COMMAND glslc ${SHADER_DIR}/shader.frag -o ${SPIRV_DIR}/frag.spv
    DEPENDS ${SHADER_DIR}/shader.frag
    COMMENT "Compiling shader.frag to frag.spv"
)

add_custom_target(Shaders
    DEPENDS ${SPIRV_DIR}/vert.spv ${SPIRV_DIR}/frag.spv
)

# Build the executable
add_executable(VulkanTest ${SRC})

target_include_directories(VulkanTest PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(VulkanTest ${GLFW_LIBRARIES} Vulkan::Vulkan dl pthread X11 Xxf86vm Xrandr Xi)

# Ensure shaders compile before building
add_dependencies(VulkanTest Shaders)

# Add custom "run" target
add_custom_target(run
    COMMAND VulkanTest
    DEPENDS VulkanTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Optional: add a clean target like in your Makefile
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -f VulkanTest
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${SPIRV_DIR}
    COMMENT "Cleaning build artifacts"
)
