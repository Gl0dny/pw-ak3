cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_GENERATOR_PLATFORM 64)

find_library(MATH_LIB m)
find_package(OpenMP)

add_executable(pw-ak3 main.c
        vars_5.h
        vars_13.h
        vars_33.h
        vars_50.h
        vars_67.h
        vars_100.h)

target_link_libraries(pw-ak3 ${MATH_LIB})
