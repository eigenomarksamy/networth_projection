cmake_minimum_required(VERSION 3.22)
project(investor)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    src/NetWorth.cpp
    src/main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src/)