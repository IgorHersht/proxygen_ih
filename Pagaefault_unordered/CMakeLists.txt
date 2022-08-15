cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++20  -rdynamic -lrt -lpthread -lz")


add_executable(untitled main.cpp gen_string.h)
