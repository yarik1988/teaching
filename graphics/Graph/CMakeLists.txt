cmake_minimum_required(VERSION 3.16)
project(Graph)
set(CMAKE_CXX_STANDARD 17)
link_directories(${CMAKE_SOURCE_DIR})

add_executable(Graph Graph.cpp)
TARGET_LINK_LIBRARIES(Graph glui_static)