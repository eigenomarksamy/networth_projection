cmake_minimum_required(VERSION 3.0)
project(libio)

add_subdirectory(file)
add_subdirectory(ipc)
add_subdirectory(cmd)
add_subdirectory(common)

include_directories(include)