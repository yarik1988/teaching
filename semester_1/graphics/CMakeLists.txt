cmake_minimum_required(VERSION 3.16)
project(GRAPHICS)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
file(GLOB imgui "imgui/*.cpp")
include(FetchContent)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "Build static libraries" FORCE)
set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL)

FetchContent_Declare(ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG master
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_MakeAvailable(SFML)
set(SFML_DIR "${SFML_BINARY_DIR}" CACHE PATH "" FORCE)
list(APPEND CMAKE_PREFIX_PATH "${SFML_BINARY_DIR}")


FetchContent_MakeAvailable(ImGui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Disable find_package in ImGui-SFML, as SFML is provided by the parent project" FORCE)
FetchContent_MakeAvailable(ImGui-SFML)

add_definitions(-DSFML_STATIC)
set(CMAKE_EXE_LINKER_FLAGS "-lwinpthread -static")
link_libraries(opengl32 winmm gdi32)

add_executable(ROTATING_SQUARE SFML_square.cpp)
add_executable(SFML_SPRITES SFML_sprites.cpp)
target_link_libraries(ROTATING_SQUARE PRIVATE SFML::Graphics SFML::Window SFML::System)
target_link_libraries(SFML_SPRITES PRIVATE SFML::Graphics SFML::Window SFML::System)

add_executable(Screener Screener.cpp glad/glad.c)
target_link_libraries(Screener PUBLIC opengl32 SFML::Graphics SFML::Window SFML::System ImGui-SFML::ImGui-SFML)

add_executable(FRAC_TREE_SFML FracTree_SFML.cpp)
target_link_libraries(FRAC_TREE_SFML SFML::Graphics SFML::Window SFML::System)