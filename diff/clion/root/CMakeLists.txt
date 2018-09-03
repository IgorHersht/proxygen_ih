cmake_minimum_required(VERSION 3.12)
project(my_solution)
set(CMAKE_VERBOSE_MAKEFILE ON)



#add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/exe_1/main_1 ${CMAKE_CURRENT_SOURCE_DIR}/exe_1/main_1/cmake-build-debug)
#add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/exe_2  ${CMAKE_CURRENT_SOURCE_DIR}/exe_2/cmake-build-debug)

add_subdirectory(main_1 )
add_subdirectory( exe_2 )
add_subdirectory( shared_lib )






