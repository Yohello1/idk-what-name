# cmake needs this line
cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "/usr/local/bin/acpp")

# Define project name
project(cv_read_save)

find_package(OpenCV REQUIRED)
find_package(AdaptiveCpp REQUIRED)
find_package(SFML COMPONENTS system window graphics)


add_executable(draw2 loadImg.cpp)
target_link_libraries(draw2 sycl sfml-system sfml-graphics sfml-window opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs sycl)

add_executable(getImg getImg.cpp)
target_link_libraries(getImg sycl sfml-system sfml-graphics sfml-window opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs sycl)

# add_sycl_to_target(TARGET run_me)
