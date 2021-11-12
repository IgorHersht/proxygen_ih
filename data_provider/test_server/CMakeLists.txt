
cmake_minimum_required(VERSION 3.12)
project(test_server)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++11")

set (BOOST "/home/ihersht/libs/boost_1.66.0")
set (PROTOBUF "/home/ihersht/libs/protobuf/protobuf-3.18.0")
set (SERVER "/home/ihersht/work/colateral/data_provider/server")

link_directories(
        ${BOOST}/lib_d
        ${PROTOBUF}/lib
        ${SERVER}/cmake-build-debug
)

link_libraries(
	server
        boost_filesystem
        boost_thread
        boost_system
        boost_chrono
		boost_program_options
        protobuf
        protoc
)

include_directories(
        ${PROJECT_SOURCE_DIR}/..
        ${BOOST}/include
        ${PROTOBUF}/include
)

add_executable( test_server
        test_server.cpp
        )

target_link_libraries(  server )
