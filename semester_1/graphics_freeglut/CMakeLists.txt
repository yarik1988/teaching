cmake_minimum_required(VERSION 3.24)
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

find_package(OpenGL REQUIRED)
add_definitions(-DFREEGLUT_STATIC)
add_definitions(-DGLEW_STATIC)
# NB: bare "-static" breaks with GCC 15.2 (CLion 2025.2 bundled MinGW):
# the static pthread/msvcrt members it pulls reference __intrinsic_setjmpex /
# __ms_vsnprintf from archives that are already scanned by then -> undefined refs
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
link_libraries(opengl32 winmm gdi32)
include_directories(PRIVATE ${freeglut_SOURCE_DIR}/include)
include_directories(PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

add_executable(DYN_PICT dyn_pict.cpp)
target_link_libraries(DYN_PICT freeglut_static opengl32 glu32)

add_executable(FLOODFILL FloodFill.cpp)
target_link_libraries(FLOODFILL freeglut_static opengl32 glu32)

add_executable(INTERVAL_INTERSECTION IntervalIntersection.cpp)
target_link_libraries(INTERVAL_INTERSECTION freeglut_static opengl32 glu32)

add_executable(TRIANGLE_SQUARE TriangleSquare.cpp)
target_link_libraries(TRIANGLE_SQUARE freeglut_static opengl32 glu32)