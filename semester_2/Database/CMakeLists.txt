cmake_minimum_required(VERSION 3.16)
project(projects)

set(CMAKE_CXX_STANDARD 14)

add_executable(write_db write_db.cpp)
add_executable(read_db read_db.cpp)
add_executable(DataBase DataBase.cpp)