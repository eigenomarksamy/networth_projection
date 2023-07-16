#include "kafka.hpp"

void testKafka() {
    std::string brokerList = "localhost:9092";
    std::string topic = "your_topic";
    std::string message = "Hello, Kafka!";

    kafka::KafkaWrapper kafkaWrapper(brokerList);
    kafkaWrapper.produce(topic, message);
}

void testKafkaCons() {
    std::string brokerList = "localhost:9092";
    std::string topic = "your_topic";
    std::string group_id = "your_group_id";
    kafka::KafkaMessageConsumer consumer(brokerList, topic, group_id);
    consumer.start();
    consumer.stop();
}