cmake_minimum_required(VERSION 3.22)
project(hash_map)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -g -O0")

add_executable(hash_map main.cpp)
