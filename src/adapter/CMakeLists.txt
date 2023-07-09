cmake_minimum_required(VERSION 3.2)
project(adapter)

add_library(adapter INTERFACE)
target_include_directories(adapter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS adapter
        ARCHIVE DESTINATION libadapter
        LIBRARY DESTINATION libadapter
        RUNTIME DESTINATION binadapter
)

install(DIRECTORY include/ DESTINATION include)