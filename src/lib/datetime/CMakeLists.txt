set(SOURCES
    datetime.cpp
)

add_library(datetime STATIC ${SOURCES})

target_include_directories(datetime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS datetime
        ARCHIVE DESTINATION lidatetime
        LIBRARY DESTINATION lidatetime
        RUNTIME DESTINATION bindatetime
)

install(DIRECTORY include/ DESTINATION include)