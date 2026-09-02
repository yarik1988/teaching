cmake_minimum_required(VERSION 3.16)
project(projects)

set(CMAKE_CXX_STANDARD 14)

add_executable(write_db write_db.cpp)
add_executable(read_db read_db.cpp)
add_executable(DataBase DataBase.cpp)

# Copy all data files next to the executables after each build
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.dat")
foreach(data_file ${DATA_FILES})
    foreach(target write_db read_db DataBase)
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${data_file}" $<TARGET_FILE_DIR:${target}>)
    endforeach()
endforeach()