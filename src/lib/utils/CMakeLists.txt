set(SOURCES
    utils.cpp
)

add_library(utils STATIC ${SOURCES})

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS utils
        ARCHIVE DESTINATION libutils
        LIBRARY DESTINATION libutils
        RUNTIME DESTINATION binutils
)

install(DIRECTORY include/ DESTINATION include)