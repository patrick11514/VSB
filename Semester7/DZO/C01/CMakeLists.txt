cmake_minimum_required(VERSION 3.5.0)
project(dzo VERSION 0.1.0)

#include(CTest)
#enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package( OpenCV REQUIRED )

add_executable(dzo main.cpp)

target_link_libraries( dzo ${OpenCV_LIBS} )

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
