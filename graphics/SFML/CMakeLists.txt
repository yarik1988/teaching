cmake_minimum_required(VERSION 3.16)
project(SFML)
set(CMAKE_CXX_STANDARD 14)
add_executable(SFML main.cpp)

if(STATIC)
    message(STATUS "STATIC_LINK")
    add_definitions(-DSFML_STATIC)
    target_link_libraries(SFML sfml-graphics-s sfml-window-s sfml-system-s opengl32 winmm gdi32)
else()
    message(STATUS "SHARED_LINK")
    target_link_libraries(SFML sfml-graphics sfml-window sfml-system)
endif()


