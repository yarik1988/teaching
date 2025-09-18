cmake_minimum_required(VERSION 3.16)
project(GRAPHICS)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
file(GLOB imgui "imgui/*.cpp")
add_definitions(-DFREEGLUT_STATIC)
add_definitions(-DGLEW_STATIC)
set(CMAKE_EXE_LINKER_FLAGS "-lwinpthread -static")
link_libraries(opengl32 winmm gdi32)

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

add_executable(GRAPH Graph.cpp ${imgui})
target_link_libraries(GRAPH freeglut opengl32 glu32 )