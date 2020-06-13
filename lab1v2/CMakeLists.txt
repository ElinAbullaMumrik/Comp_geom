cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab1v2 main.cpp NetPBM.h PPM.h PBM.h)