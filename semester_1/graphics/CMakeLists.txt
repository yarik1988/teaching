cmake_minimum_required(VERSION 3.16)
project(GRAPHICS)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
file(GLOB imgui "imgui/*.cpp")
include(FetchContent)
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
FetchContent_MakeAvailable(ImGui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(ImGui-SFML)

add_definitions(-DSFML_STATIC)
add_definitions(-DFREEGLUT_STATIC)
add_definitions(-DGLEW_STATIC)
set(CMAKE_EXE_LINKER_FLAGS "-lwinpthread -static")
link_libraries(opengl32 winmm gdi32)


add_executable(ROTATING_SQUARE SFML_square.cpp)
add_executable(SFML_SPRITES SFML_sprites.cpp)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
target_link_libraries(ROTATING_SQUARE PRIVATE SFML::Graphics SFML::Window SFML::System)
target_link_libraries(SFML_SPRITES PRIVATE SFML::Graphics SFML::Window SFML::System)

add_executable(Screener Screener.cpp glad/glad.c)
target_link_libraries(Screener PUBLIC opengl32 SFML::Graphics SFML::Window SFML::System ImGui-SFML::ImGui-SFML)


add_executable(3D 3D.cpp)
target_link_libraries(3D freeglut opengl32)
add_executable(DYN_PICT dyn_pict.cpp)
target_link_libraries(DYN_PICT freeglut opengl32 glu32)

add_executable(FLOODFILL FloodFill.cpp)
target_link_libraries(FLOODFILL freeglut opengl32 glu32)

add_executable(GAME_LIFE GameLife.cpp)
target_link_libraries(GAME_LIFE freeglut opengl32 glu32)

add_executable(INTERVAL_INTERSECTION IntervalIntersection.cpp)
target_link_libraries(INTERVAL_INTERSECTION freeglut opengl32 glu32)

add_executable(TRIANGLE_SQUARE TriangleSquare.cpp)
target_link_libraries(TRIANGLE_SQUARE freeglut opengl32 glu32)

add_executable(FRAC_TREE FracTree.cpp)
target_link_libraries(FRAC_TREE freeglut opengl32 glu32)

add_executable(GRAPH Graph.cpp ${imgui})
target_link_libraries(GRAPH freeglut opengl32 glu32 )