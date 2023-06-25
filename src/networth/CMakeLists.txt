set(SOURCES
    networth.cpp
)

add_library(networthprojector STATIC ${SOURCES})

target_include_directories(networthprojector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(networthprojector PRIVATE utils)

install(TARGETS networthprojector
        ARCHIVE DESTINATION libnetworthprojector
        LIBRARY DESTINATION libnetworthprojector
        RUNTIME DESTINATION binnetworthprojector
)

install(DIRECTORY include/ DESTINATION include)