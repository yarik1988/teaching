cmake_minimum_required(VERSION 3.16)
project(list)

set(CMAKE_CXX_STANDARD 14)

add_executable(list simple_list.cpp)
add_executable(cyclic_list cyclic_list.cpp)