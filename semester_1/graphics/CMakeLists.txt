cmake_minimum_required(VERSION 3.16)
project(GRAPHICS)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
include(FetchContent)

message(STATUS "C++ compiler id: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_LINKER: ${CMAKE_LINKER}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

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
target_compile_definitions(ImGui-SFML PRIVATE NDEBUG)

add_definitions(-DSFML_STATIC)

# --- Executables ---
add_executable(ROTATING_SQUARE SFML_square.cpp)
target_link_libraries(ROTATING_SQUARE PRIVATE SFML::Graphics SFML::Window SFML::System)

add_executable(SFML_SPRITES SFML_sprites.cpp)
target_link_libraries(SFML_SPRITES PRIVATE SFML::Graphics SFML::Window SFML::System)

add_executable(Screener Screener.cpp glad/glad.c)
target_link_libraries(Screener PRIVATE SFML::Graphics SFML::Window SFML::System ImGui-SFML::ImGui-SFML)

add_executable(FRAC_TREE_SFML FracTree_SFML.cpp)
target_link_libraries(FRAC_TREE_SFML PRIVATE SFML::Graphics SFML::Window SFML::System)

add_custom_target(sem1_graphics DEPENDS ROTATING_SQUARE SFML_SPRITES Screener FRAC_TREE_SFML)

# --- Platform-specific settings ---
if (WIN32)
    # Apply MinGW static flags only when using GNU or Clang toolchains targeting Windows.
    # Use generator-expression so this won't affect MSVC.
    foreach(tgt ROTATING_SQUARE SFML_SPRITES Screener FRAC_TREE_SFML)
        if (TARGET ${tgt})
            target_link_options(${tgt} PRIVATE
                    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<PLATFORM_ID:Windows>>:
                    -static;-static-libgcc;-static-libstdc++;-Wl,-Bstatic;-lwinpthread;-Wl,-Bdynamic
                    >
            )
        endif()
    endforeach()

    # system libs (you can also add these per-target via target_link_libraries)
    link_libraries(opengl32 winmm gdi32)
elseif(APPLE)
    # macOS uses frameworks
    find_library(COREFOUNDATION_FRAMEWORK Foundation)
    find_library(APPKIT_FRAMEWORK AppKit)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CARBON_FRAMEWORK Carbon)

    link_libraries(
            "-framework OpenGL"
            ${COREFOUNDATION_FRAMEWORK}
            ${APPKIT_FRAMEWORK}
            ${IOKIT_FRAMEWORK}
            ${CARBON_FRAMEWORK}
    )
elseif(UNIX)
    # Linux: link X11, pthread, etc. if needed
    find_package(Threads REQUIRED)
    link_libraries(${CMAKE_THREAD_LIBS_INIT} GL X11)
endif()