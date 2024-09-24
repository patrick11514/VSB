cmake_minimum_required(VERSION 3.10)
project(ZPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE LEGACY)

add_compile_options(-Wall -Wextra -pedantic)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE glfw glm::glm ${OPENGL_gl_LIBRARY})
