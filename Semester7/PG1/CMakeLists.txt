cmake_minimum_required(VERSION 3.10)
project(PG1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic)

find_package(imgui CONFIG REQUIRED)
find_package(embree 4 CONFIG REQUIRED)
find_library(FREEIMAGE_LIBRARY freeimage)
find_package(glm REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE embree imgui::imgui ${FREEIMAGE_LIBRARY} glm::glm)