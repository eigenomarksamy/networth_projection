include(cmake/yaml-cpp.cmake)

set(SOURCES
    file_generator.cpp
    logger.cpp
    yml_prsr.cpp
)

add_library(fileio STATIC ${SOURCES})

target_link_libraries(fileio PRIVATE yaml-cpp)

target_include_directories(fileio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(fileio PRIVATE ${yaml-cpp_SOURCE_DIR}/include)

install(TARGETS fileio
        ARCHIVE DESTINATION libfileio
        LIBRARY DESTINATION libfileio
        RUNTIME DESTINATION binfileio
)

install(DIRECTORY include/ DESTINATION include)