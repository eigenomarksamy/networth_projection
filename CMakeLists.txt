cmake_minimum_required(VERSION 3.22)
project(investor)
set(CMAKE_CXX_STANDARD 20)

set(SRC
    src/main.cpp
)

include_directories(lib/include)

add_subdirectory(src/lib)
add_subdirectory(src/adapter)
add_subdirectory(src/db_manager)
add_subdirectory(src/portfolio)
add_subdirectory(src/io)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    utils
    adapter
    db_manager
    fileio
    cmdio
    commonio
    portfoliomanager
    kafka
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    src/io/
)

set(DEPL_DEST
    "${CMAKE_BINARY_DIR}/../bin"
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${DEPL_DEST}
)