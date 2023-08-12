cmake_minimum_required(VERSION 3.2)
project(db_manager)

set(SOURCES
    strategy_db.cpp
)

add_library(db_manager STATIC ${SOURCES})

target_include_directories(db_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(db_manager PRIVATE sqlite3)

install(TARGETS db_manager
        ARCHIVE DESTINATION libdb_manager
        LIBRARY DESTINATION libdb_manager
        RUNTIME DESTINATION bindb_manager
)

install(DIRECTORY include/ DESTINATION include)