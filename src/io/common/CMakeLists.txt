set(SOURCES
    data_adapter.cpp
    conf_resolver.cpp
)

add_library(commonio STATIC ${SOURCES})

target_include_directories(commonio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(commonio
    PRIVATE
    utils networthprojector mortgagecalculator portfoliomanager
)

install(TARGETS commonio
        ARCHIVE DESTINATION libcommonio
        LIBRARY DESTINATION libcommonio
        RUNTIME DESTINATION bincommonio
)

install(DIRECTORY include/ DESTINATION include)
