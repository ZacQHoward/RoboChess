cmake_minimum_required(VERSION 3.28)
project(RoboChess)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/states.cpp
    src/fen_parser.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_compile_definitions(GUI_PATH="${PROJECT_SOURCE_DIR}/gui")

