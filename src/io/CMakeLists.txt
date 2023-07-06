cmake_minimum_required(VERSION 3.0)
project(libio)

add_subdirectory(common)
add_subdirectory(file)
add_subdirectory(ipc)
add_subdirectory(cmd)

include_directories(include)