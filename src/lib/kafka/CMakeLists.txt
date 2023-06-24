find_package(CppKafka REQUIRED)

set(SOURCES
    kafka.cpp
    kafka_manager.cpp
)

add_library(kafka STATIC ${SOURCES})

target_include_directories(kafka PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(kafka PRIVATE cppkafka utils)

install(TARGETS kafka
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

target_link_libraries(kafka PRIVATE cppkafka)
