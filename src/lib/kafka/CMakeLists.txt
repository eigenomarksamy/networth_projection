find_package(CppKafka REQUIRED)

set(SOURCES
    kafka_consumer.cpp
    kafka_producer.cpp
)

add_library(kafka STATIC ${SOURCES})

target_include_directories(kafka PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(kafka PRIVATE cppkafka utils)

install(TARGETS kafka
        ARCHIVE DESTINATION libkafka
        LIBRARY DESTINATION libkafka
        RUNTIME DESTINATION binkafka
)

install(DIRECTORY include/ DESTINATION include)

target_link_libraries(kafka PRIVATE cppkafka)
