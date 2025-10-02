cmake_minimum_required(VERSION 3.16)
project(dyncompile)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(TINYCC
        GIT_REPOSITORY https://github.com/TinyCC/tinycc.git
        GIT_TAG mob
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_MakeAvailable(TINYCC)

# Define a custom command to build TCC (only when output doesn't exist)
if(WIN32)
    add_custom_command(
            OUTPUT "${tinycc_SOURCE_DIR}/win32/libtcc.dll"
            COMMAND ${CMAKE_COMMAND} -E chdir "${tinycc_SOURCE_DIR}/win32"
            cmd /c build-tcc.bat
            COMMENT "Building TinyCC from source"
            VERBATIM
    )

    # Copy the DLL to build directory
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/libtcc.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${tinycc_SOURCE_DIR}/win32/libtcc.dll"
            "${CMAKE_CURRENT_BINARY_DIR}/libtcc.dll"
            DEPENDS "${tinycc_SOURCE_DIR}/win32/libtcc.dll"
            COMMENT "Copying libtcc.dll to build directory"
    )

    # Create a target that depends on the DLL
    add_custom_target(build_tcc DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/libtcc.dll")
endif()

link_directories(${tinycc_SOURCE_DIR}/win32)
include_directories(${tinycc_SOURCE_DIR}/win32/libtcc)

add_executable(SIMPLE_TEST main.cpp)
add_executable(LIBTCC_TEST libtcc_test.c)

# Make executables depend on build_tcc
if(WIN32)
    add_dependencies(SIMPLE_TEST build_tcc)
    add_dependencies(LIBTCC_TEST build_tcc)
endif()

# Link against the DLL
target_link_libraries(SIMPLE_TEST PRIVATE ${tinycc_SOURCE_DIR}/win32/libtcc.dll)
target_link_libraries(LIBTCC_TEST PRIVATE ${tinycc_SOURCE_DIR}/win32/libtcc.dll)

add_compile_definitions(TINYCC_SOURCE_DIR="${tinycc_SOURCE_DIR}")
add_custom_target(dynamic_compilation DEPENDS SIMPLE_TEST LIBTCC_TEST)

