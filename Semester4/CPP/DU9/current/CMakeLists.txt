cmake_minimum_required(VERSION 3.8)
project(practice09)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
add_compile_options(-Wall -Wextra -pedantic)

add_executable(polymorphism polymorphism.cpp)
add_executable(constexpr constexpr.cpp)
add_executable(array array.cpp)
add_executable(functional functional.cpp)
add_executable(variadic variadic.cpp)
add_executable(parser parser.cpp)
