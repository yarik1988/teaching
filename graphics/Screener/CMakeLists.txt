cmake_minimum_required(VERSION 3.16)
project(Screener)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
add_executable(Screener Screener.cpp)
target_link_libraries(Screener glew32)