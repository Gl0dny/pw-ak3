cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)

find_library(MATH_LIB m)

add_executable(pw-ak3 main.c
        vars_2.h
        vars_3.h
        vars_4.h
        vars_5.h)

target_link_libraries(pw-ak3 ${MATH_LIB})
