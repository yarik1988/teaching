cmake_minimum_required(VERSION 3.16)
project(teaching)
add_subdirectory(dynamic_compilation)

# Automatically add all subdirectories in semester_1
file(GLOB SEM1_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/semester_1/*")
foreach(dir ${SEM1_DIRS})
    if(IS_DIRECTORY ${dir})
        get_filename_component(subdir ${dir} NAME)
        add_subdirectory("semester_1/${subdir}")
    endif()
endforeach()

add_subdirectory("semester_2/Lists")