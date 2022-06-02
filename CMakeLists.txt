cmake_minimum_required(VERSION 3.1.0)
project(chess)

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIRECTORY source)
set(INCLUDE_DIRECTORY headers)

set(SOURCES
    ${SRC_DIRECTORY}/Square.cpp
    ${SRC_DIRECTORY}/Board.cpp
)

include_directories(${INCLUDE_DIRECTORY})
add_executable(${PROJECT_NAME}
    ${SRC_DIRECTORY}/main.cpp
    ${SOURCES})

