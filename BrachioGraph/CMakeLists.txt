cmake_minimum_required(VERSION 3.10)

project(opencv_demo)

add_executable(BrachioGraph BrachioGraph.cpp)

set(OPENCV_INCLUDE_DIR /usr/local/include/opencv4/)
set(OPENCV_LIB_DIR /usr/local/lib/)

message(STATUS "OpenCV library path: ${OPENCV_LIB_DIR}")

# set include directory
target_include_directories(BrachioGraph PUBLIC "${OPENCV_INCLUDE_DIR}")

# set library directory
target_link_directories(BrachioGraph PUBLIC "${OPENCV_LIB_DIR}")

# link specific object files we need
target_link_libraries(BrachioGraph PUBLIC opencv_imgcodecs opencv_core opencv_imgproc PillowResize)

#find_package(PillowResize REQUIRED)