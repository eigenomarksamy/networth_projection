set(SOURCES
    mortgage.cpp
)

add_library(mortgagecalculator STATIC ${SOURCES})

target_include_directories(mortgagecalculator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(mortgagecalculator PRIVATE utils)

install(TARGETS mortgagecalculator
        ARCHIVE DESTINATION libmortgagecalculator
        LIBRARY DESTINATION libmortgagecalculator
        RUNTIME DESTINATION binmortgagecalculator
)

install(DIRECTORY include/ DESTINATION include)