set(SOURCES
    activation.cpp
)

add_library(interfaces STATIC ${SOURCES})

target_include_directories(interfaces PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(interfaces PRIVATE utils networthprojector mortgagecalculator adapter commonio fileio)

install(TARGETS interfaces
        ARCHIVE DESTINATION libinterfaces
        LIBRARY DESTINATION libinterfaces
        RUNTIME DESTINATION bininterfaces
)

install(DIRECTORY include/ DESTINATION include)