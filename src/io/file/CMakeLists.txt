set(SOURCES
    file_manager.cpp
    logger.cpp
)

add_library(fileio STATIC ${SOURCES})

target_include_directories(fileio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS fileio
        ARCHIVE DESTINATION libfileio
        LIBRARY DESTINATION libfileio
        RUNTIME DESTINATION binfileio
)

install(DIRECTORY include/ DESTINATION include)