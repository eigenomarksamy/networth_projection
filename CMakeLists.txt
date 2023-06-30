cmake_minimum_required(VERSION 3.22)
project(investor)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    src/io/input_handler.cpp
    src/io/continuous_input.cpp
    src/io/data_adapter.cpp
    src/main.cpp
)

include_directories(lib/include)

add_subdirectory(src/lib)
add_subdirectory(src/networth)
add_subdirectory(src/mortgage)
add_subdirectory(src/portfolio)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME}
    utils
    networthprojector
    mortgagecalculator
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