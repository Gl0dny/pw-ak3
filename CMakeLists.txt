cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_GENERATOR_PLATFORM 64)

find_library(MATH_LIB m)
find_package(OpenMP)

# 5
add_executable(ak3-5 main.c)
target_link_libraries(ak3-5 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-5 PRIVATE SIZE=5)
#//

# 10
add_executable(ak3-13 main.c)
target_link_libraries(ak3-13 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-13 PRIVATE SIZE=13)
#//

# 33
add_executable(ak3-33 main.c)
target_link_libraries(ak3-33 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-33 PRIVATE SIZE=33)
#//

# 50
add_executable(ak3-50 main.c)
target_link_libraries(ak3-50 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-50 PRIVATE SIZE=50)
#//

# 67
add_executable(ak3-67 main.c)
target_link_libraries(ak3-67 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-67 PRIVATE SIZE=67)
#//

# 100
add_executable(ak3-100 main.c)
target_link_libraries(ak3-100 PUBLIC ${MATH_LIB} OpenMP::OpenMP_C)
target_compile_definitions(ak3-100 PRIVATE SIZE=100)
#//
