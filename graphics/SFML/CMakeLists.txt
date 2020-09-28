cmake_minimum_required(VERSION 3.16)
project(SFML)

set(CMAKE_CXX_STANDARD 14)

add_executable(SFML main.cpp)

target_link_libraries(SFML sfml-graphics sfml-window sfml-system)