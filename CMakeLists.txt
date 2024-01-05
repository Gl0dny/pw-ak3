cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_GENERATOR_PLATFORM 64)

find_library(MATH_LIB m)
find_package(OpenMP)

add_executable(ak3-sekwencyjna-5 main.c)
target_link_libraries(ak3-sekwencyjna-5 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-5 PRIVATE SIZE=5)

add_executable(ak3-sekwencyjna-13 main.c)
target_link_libraries(ak3-sekwencyjna-13 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-13 PRIVATE SIZE=13)

add_executable(ak3-sekwencyjna-33 main.c)
target_link_libraries(ak3-sekwencyjna-33 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-33 PRIVATE SIZE=33)

add_executable(ak3-sekwencyjna-50 main.c)
target_link_libraries(ak3-sekwencyjna-50 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-50 PRIVATE SIZE=50)

add_executable(ak3-sekwencyjna-67 main.c)
target_link_libraries(ak3-sekwencyjna-67 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-67 PRIVATE SIZE=67)

add_executable(ak3-sekwencyjna-100 main.c)
target_link_libraries(ak3-sekwencyjna-100 ${MATH_LIB})
target_compile_definitions(ak3-sekwencyjna-100 PRIVATE SIZE=100)


