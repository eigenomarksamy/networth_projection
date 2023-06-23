cmake_minimum_required(VERSION 3.22)
project(investor)
set(CMAKE_CXX_STANDARD 20)

find_package(CppKafka REQUIRED)

set(SOURCE_FILES
    src/comm/kafka_manager.cpp
    src/networth/networth.cpp
    src/mortgage/mortgage.cpp
    src/portfolio/portfolio.cpp
    src/files/file_manager.cpp
    src/io/input_handler.cpp
    src/io/continuous_input.cpp
    src/io/data_adapter.cpp
    src/utils/utils.cpp
    src/main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    src/
    src/utils/
    src/networth/
    src/mortgage/
    src/portfolio/
    src/investment/
    src/files/
    src/io/
    src/comm/
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cppkafka)

set(DEPL_DEST
    "${CMAKE_BINARY_DIR}/../bin"
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${DEPL_DEST}
)