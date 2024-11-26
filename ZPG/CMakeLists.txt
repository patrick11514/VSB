cmake_minimum_required(VERSION 3.10)
project(ZPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE LEGACY)

find_library(SOIL2_LIBRARY soil2 PATHS /usr/lib)

add_compile_options(-Wall -Wextra -pedantic)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(SOIL2 REQUIRED)
find_package(assimp REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE glfw glm::glm GL ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${SOIL2_LIBRARY} assimp)
