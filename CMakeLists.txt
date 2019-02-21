cmake_minimum_required(VERSION 3.13)
project(gujia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -Wall")

add_executable(gujia main.cpp src/gujia.h)