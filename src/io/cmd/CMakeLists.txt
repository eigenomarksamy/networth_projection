set(SOURCES
    user_io.cpp
    static_io.cpp
)

add_library(cmdio STATIC ${SOURCES})

target_include_directories(cmdio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cmdio PRIVATE utils commonio)

install(TARGETS cmdio
        ARCHIVE DESTINATION libcmdio
        LIBRARY DESTINATION libcmdio
        RUNTIME DESTINATION bincmdio
)

install(DIRECTORY include/ DESTINATION include)