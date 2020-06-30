cmake_minimum_required(VERSION 3.16)
project(dyncompile)
set(CMAKE_CXX_STANDARD 17)

if ($ENV{COMPILER} STREQUAL VS)
    link_directories(libVS)
    elseif ($ENV{COMPILER} STREQUAL MINGW)
    link_directories(libGCC)
endif()

add_executable(dyncompile main.cpp)
#add_library(dyncompile main.cpp)
target_link_libraries(dyncompile libtcc)
