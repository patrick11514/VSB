cmake_minimum_required(VERSION 3.5.0)
project(dzo VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Only request the modules you actually use (add 'video' or others if needed)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs)
find_package(OpenMP)

add_executable(dzo main.cpp)

target_link_libraries(dzo ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
