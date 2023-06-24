set(SOURCES
    utils.cpp
)

add_library(utils STATIC ${SOURCES})

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS utils
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

target_link_libraries(utils PRIVATE cppkafka)