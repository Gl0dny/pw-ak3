cmake_minimum_required(VERSION 3.22)
project(pw-ak3 C)

set(CMAKE_C_STANDARD 11)
set(MPI_C_COMPILER mpicc)

find_library(MATH_LIB m)
find_package(OpenMP)
find_package(MPI REQUIRED)

add_executable(ak3-5 main.c)
target_link_libraries(ak3-5 ${MATH_LIB})
target_compile_definitions(ak3-5 PRIVATE SIZE=5)
target_link_libraries(ak3-5 MPI::MPI_C)  

add_executable(ak3-13 main.c)
target_link_libraries(ak3-13 ${MATH_LIB})
target_compile_definitions(ak3-13 PRIVATE SIZE=13)
target_link_libraries(ak3-13 MPI::MPI_C)  

add_executable(ak3-33 main.c)
target_link_libraries(ak3-33 ${MATH_LIB})
target_compile_definitions(ak3-33 PRIVATE SIZE=33)
target_link_libraries(ak3-33 MPI::MPI_C)  

add_executable(ak3-50 main.c)
target_link_libraries(ak3-50 ${MATH_LIB})
target_compile_definitions(ak3-50 PRIVATE SIZE=50)
target_link_libraries(ak3-50 MPI::MPI_C)  

add_executable(ak3-67 main.c)
target_link_libraries(ak3-67 ${MATH_LIB})
target_compile_definitions(ak3-67 PRIVATE SIZE=67)
target_link_libraries(ak3-67 MPI::MPI_C)  

add_executable(ak3-100 main.c)
target_link_libraries(ak3-100 ${MATH_LIB})
target_compile_definitions(ak3-100 PRIVATE SIZE=100)
target_link_libraries(ak3-100 MPI::MPI_C)  