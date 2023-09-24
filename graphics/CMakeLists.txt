cmake_minimum_required(VERSION 3.16)
project(GRAPHICS)
set (STATIC true) # to link statically
set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)
file(GLOB imgui "imgui/*.cpp")


add_executable(ROTATING_SQUARE SFML_square.cpp)
add_executable(SFML_SPRITES SFML_sprites.cpp)
if(STATIC)
    message(STATUS "STATIC_LINK")
    add_definitions(-DSFML_STATIC)
    add_definitions(-DFREEGLUT_STATIC)
    add_definitions(-DGLEW_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
    link_libraries(opengl32 winmm gdi32)
    target_link_libraries(ROTATING_SQUARE sfml-graphics-s sfml-window-s sfml-system-s opengl32 winmm gdi32)
    target_link_libraries(SFML_SPRITES sfml-graphics-s sfml-window-s sfml-system-s opengl32 winmm gdi32)
else()
    message(STATUS "SHARED_LINK")
    target_link_libraries(ROTATING_SQUARE sfml-graphics sfml-window sfml-system)
    target_link_libraries(SFML_SPRITES sfml-graphics sfml-window sfml-system)
endif()

add_executable(Screener Screener.cpp ${imgui})
target_link_libraries(Screener freeglut opengl32 glew32)
add_executable(3D 3D.cpp)
target_link_libraries(3D freeglut opengl32)
add_executable(DYN_PICT dyn_pict.cpp)
target_link_libraries(DYN_PICT freeglut opengl32 glu32)

add_executable(FLOODFILL FloodFill.cpp)
target_link_libraries(FLOODFILL freeglut opengl32 glu32)

add_executable(INTERVAL_INTERSECTION IntervalIntersection.cpp)
target_link_libraries(INTERVAL_INTERSECTION freeglut opengl32 glu32)

add_executable(TRIANGLE_SQUARE TriangleSquare.cpp)
target_link_libraries(TRIANGLE_SQUARE freeglut opengl32 glu32)

add_executable(GRAPH Graph.cpp ${imgui})
target_link_libraries(GRAPH freeglut opengl32 glu32 )

