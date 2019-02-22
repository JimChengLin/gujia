cmake_minimum_required(VERSION 3.8)
project(gujia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(gujia main.cpp
        src/anet.c
        src/anet.h
        src/fmacros.h
        src/gujia.h
        src/gujia_epoll_impl.h
        src/gujia_impl.h
        src/gujia_kqueue_impl.h)