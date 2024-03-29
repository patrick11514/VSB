cmake_minimum_required(VERSION 3.8)
project(Projekt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)

if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_compile_options(-Wall -Wextra -pedantic)

set(UTILS "utils/argParser.cpp" "utils/logger.cpp" "utils/threadPool.cpp" "utils/fileRead.cpp" "utils/mimeType.cpp" "utils/decode.cpp")
set(SERVER "server/server.cpp" "server/payload.cpp" "server/response.cpp")
set(MODES "server/modes/mainMode.cpp" "server/modes/devMode.cpp")
add_executable(main main.cpp socket/socket.cpp ${UTILS} ${SERVER} ${MODES})
