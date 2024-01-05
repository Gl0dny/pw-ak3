cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)

find_library(MATH_LIB m)
find_package(OpenMP)

add_executable(ak3-5 main.c)
target_link_libraries(ak3-5 ${MATH_LIB})
target_compile_definitions(ak3-5 PRIVATE SIZE=5)

add_executable(ak3-13 main.c)
target_link_libraries(ak3-13 ${MATH_LIB})
target_compile_definitions(ak3-13 PRIVATE SIZE=13)

add_executable(ak3-33 main.c)
target_link_libraries(ak3-33 ${MATH_LIB})
target_compile_definitions(ak3-33 PRIVATE SIZE=33)

add_executable(ak3-50 main.c)
target_link_libraries(ak3-50 ${MATH_LIB})
target_compile_definitions(ak3-50 PRIVATE SIZE=50)

add_executable(ak3-67 main.c)
target_link_libraries(ak3-67 ${MATH_LIB})
target_compile_definitions(ak3-67 PRIVATE SIZE=67)

add_executable(ak3-100 main.c)
target_link_libraries(ak3-100 ${MATH_LIB})
target_compile_definitions(ak3-100 PRIVATE SIZE=100)


