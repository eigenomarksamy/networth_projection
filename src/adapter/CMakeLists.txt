cmake_minimum_required(VERSION 3.2)
project(adapter)

set(SOURCES
    str_adapter.cpp
)

add_library(adapter STATIC ${SOURCES})
target_include_directories(adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS adapter
        ARCHIVE DESTINATION libadapter
        LIBRARY DESTINATION libadapter
        RUNTIME DESTINATION binadapter
)

install(DIRECTORY include/ DESTINATION include)