cmake_minimum_required(VERSION 3.28)
project(RoboChess)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/states.cpp
)
