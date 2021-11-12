cmake_minimum_required(VERSION 3.12)
project(exe_2)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++11")

add_subdirectory(server)
add_subdirectory( client )
add_subdirectory( test_server )
add_subdirectory( serialization )









