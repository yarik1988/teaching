cmake_minimum_required(VERSION 3.16)
project(Sparse)

set(CMAKE_CXX_STANDARD 14)

add_executable(det SparseMatrixDeterminant.cpp
        aux_fun.h)
