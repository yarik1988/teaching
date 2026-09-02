#Автор: Точоний Володимир
cmake_minimum_required(VERSION 3.16)
project(REVERSI)
set(CMAKE_CXX_STANDARD 17)
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

# semester_1/graphics may have already fetched and built SFML in the same build
# tree — a second add_subdirectory would collide on the sfml-* targets, so reuse
# it in that case. Must be added to the parent CMakeLists AFTER semester_1.
if(NOT TARGET SFML::Graphics)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.0.2
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
            SYSTEM)

    FetchContent_MakeAvailable(SFML)
    set(SFML_DIR "${SFML_BINARY_DIR}" CACHE PATH "" FORCE)
    list(APPEND CMAKE_PREFIX_PATH "${SFML_BINARY_DIR}")
endif()

add_definitions(-DSFML_STATIC)

# --- Executable ---
# All classes are header-only; the only real TUs are main and the AI evaluators.
add_executable(REVERSI
        laba5_reversi.cpp
        AIStatic.cpp
        AIGenius.cpp
        AIDynamic.cpp
)
target_link_libraries(REVERSI PRIVATE SFML::Graphics SFML::Window SFML::System)

add_custom_target(student_projects_reversi DEPENDS REVERSI)

# --- Platform-specific settings ---
if (WIN32)
    # Apply MinGW static flags only when using GNU or Clang toolchains targeting Windows.
    # Use generator-expression so this won't affect MSVC.
    foreach(tgt REVERSI)
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
