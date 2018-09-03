cmake_minimum_required(VERSION 3.12)
project(shared_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")


add_library(shared_lib SHARED library_shared.cpp library_shared.h)




