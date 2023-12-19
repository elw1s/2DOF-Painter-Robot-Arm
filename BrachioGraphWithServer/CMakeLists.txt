cmake_minimum_required(VERSION 3.10)

project(opencv_demo)

add_executable(server mainserver.cpp server_code1.cpp)


# link specific object files we need
target_link_libraries(server PUBLIC pigpio)


