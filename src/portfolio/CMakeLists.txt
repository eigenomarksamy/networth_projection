set(SOURCES
    portfolio.cpp
)

add_library(portfoliomanager STATIC ${SOURCES})

target_include_directories(portfoliomanager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS portfoliomanager
        ARCHIVE DESTINATION libportfoliomanager
        LIBRARY DESTINATION libportfoliomanager
        RUNTIME DESTINATION binportfoliomanager
)

install(DIRECTORY include/ DESTINATION include)