set(SOURCES
    file_manager.cpp
    logger.cpp
)

add_library(filemanager STATIC ${SOURCES})

target_include_directories(filemanager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS filemanager
        ARCHIVE DESTINATION libfilemanager
        LIBRARY DESTINATION libfilemanager
        RUNTIME DESTINATION binfilemanager
)

install(DIRECTORY include/ DESTINATION include)