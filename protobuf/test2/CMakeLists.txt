
cmake_minimum_required(VERSION 3.12)
project(exe_2)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++11")

set (BOOST "/home/ihersht/libs/boost")
set (PROTOBUF "/home/ihersht/libs/protobuf/protobuf-3.18.0")
link_directories(
        ${BOOST}/lib_d
        ${PROTOBUF}/lib
)

link_libraries(
        boost_filesystem
        boost_thread
        boost_system
        boost_chrono
        protobuf
        protoc
)

include_directories(
        ${PROJECT_SOURCE_DIR}/..
        ${BOOST}/include
        ${PROTOBUF}/include
)

add_executable( exe_2
        main.cpp
        types.pb.cc
        message.pb.cc
        )
target_link_libraries( exe_2 )
