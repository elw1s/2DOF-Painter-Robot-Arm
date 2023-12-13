cmake_minimum_required(VERSION 3.10)

project(opencv_demo)

add_executable(test test.cpp)


# link specific object files we need
target_link_libraries(test PUBLIC pigpio)


