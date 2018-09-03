
cmake_minimum_required(VERSION 3.12)
project(exe_2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

set (BOOST "/home/ihersht/libs/boost")
link_directories(
        ${BOOST}/lib_d

)

link_libraries(
        boost_filesystem
        boost_thread
        boost_system
        boost_chrono
)

include_directories(
        ${BOOST}/include

)


set (ROOT_DIR "/home/ihersht/projects/clion/root")

add_subdirectory(  ${ROOT_DIR}/shared_lib  ${ROOT_DIR}/shared_lib/cmake-build-debug)

include_directories(
        ${ROOT_DIR}

)

add_executable( exe_2
        main.cpp

        )
target_link_libraries( exe_2 shared_lib )
