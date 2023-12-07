cmake_minimum_required(VERSION 3.10)

project(opencv_demo)

add_executable(test test.cpp)

set(OPENCV_INCLUDE_DIR /usr/local/include/opencv4/)
set(OPENCV_LIB_DIR /usr/local/lib/)

message(STATUS "OpenCV library path: ${OPENCV_LIB_DIR}")

# set include directory
target_include_directories(test PUBLIC "${OPENCV_INCLUDE_DIR}")

# set library directory
target_link_directories(test PUBLIC "${OPENCV_LIB_DIR}")

# link specific object files we need
target_link_libraries(test PUBLIC opencv_imgcodecs opencv_core opencv_imgproc PillowResize pigpio)

find_package(PillowResize REQUIRED)