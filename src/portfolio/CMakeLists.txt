set(SOURCES
    portfolio.cpp
    portfolio_appl.cpp
    portfolio_file.cpp
    portfolio_db.cpp
)

add_library(portfoliomanager STATIC ${SOURCES})

target_include_directories(portfoliomanager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
    portfoliomanager
    PRIVATE
    utils
    cmdio
    fileio
    db_manager
)

install(TARGETS portfoliomanager
        ARCHIVE DESTINATION libportfoliomanager
        LIBRARY DESTINATION libportfoliomanager
        RUNTIME DESTINATION binportfoliomanager
)

install(DIRECTORY include/ DESTINATION include)