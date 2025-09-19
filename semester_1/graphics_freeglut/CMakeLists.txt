cmake_minimum_required(VERSION 4.0)
project(GRAPHICS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
include(FetchContent)
FetchContent_Declare(
        freeglut
        GIT_REPOSITORY https://github.com/freeglut/freeglut.git
        GIT_TAG        v3.6.0
)

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL)

FetchContent_MakeAvailable(freeglut)
FetchContent_MakeAvailable(ImGui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp # Optional, but useful for testing
)

find_package(OpenGL REQUIRED)
add_definitions(-DFREEGLUT_STATIC)
add_definitions(-DGLEW_STATIC)
set(CMAKE_EXE_LINKER_FLAGS "-lwinpthread -static")
link_libraries(opengl32 winmm gdi32)
include_directories(PRIVATE ${freeglut_SOURCE_DIR}/include)
include_directories(PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
add_executable(3D 3D.cpp)
target_link_libraries(3D freeglut_static opengl32)

add_executable(DYN_PICT dyn_pict.cpp)
target_link_libraries(DYN_PICT freeglut_static opengl32 glu32)

add_executable(FLOODFILL FloodFill.cpp)
target_link_libraries(FLOODFILL freeglut_static opengl32 glu32)

add_executable(GAME_LIFE GameLife.cpp)
target_link_libraries(GAME_LIFE freeglut_static opengl32 glu32)

add_executable(INTERVAL_INTERSECTION IntervalIntersection.cpp)
target_link_libraries(INTERVAL_INTERSECTION freeglut_static opengl32 glu32)

add_executable(TRIANGLE_SQUARE TriangleSquare.cpp)
target_link_libraries(TRIANGLE_SQUARE freeglut_static opengl32 glu32)

add_executable(GRAPH Graph.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glut.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp)
target_link_libraries(GRAPH imgui freeglut_static opengl32 glu32)