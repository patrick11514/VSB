cmake_minimum_required(VERSION 3.8)
project(tasks01)

set(CMAKE_CXX_STANDARD 17)

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)

if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_compile_options(-Wall -Wextra -pedantic -Werror=uninitialized -Werror=vla -Werror=return-type -Wno-unused-results)

# Use main.cpp to call and debug your functions
add_executable(tasks main.cpp tasks.cpp)

# Use the `tests` executable to run unit tests
add_executable(tests tests.cpp tasks.cpp)
