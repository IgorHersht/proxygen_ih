cmake_minimum_required(VERSION 3.22)
project(integral_value)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wno-sign-compare -Wno-unused-variable -g -O0")

add_executable(integral_value main.cpp)
