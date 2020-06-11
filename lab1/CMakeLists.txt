cmake_minimum_required(VERSION 3.15)
project(Comp_geom C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_COMPILER /usr/bin/clang)

add_executable(Comp_geom main.c work_with_PNM.c work_with_PNM.h)