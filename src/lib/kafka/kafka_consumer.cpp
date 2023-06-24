#include <thread>
#include "kafka_consumer.hpp"

void kafka::KafkaMessageConsumer::start() {
    m_consumer->subscribe({ m_topic });
    while (true) {
        cppkafka::Message message = m_consumer->poll();
        if (message) {
            processMessage(message);
        }
    }
}

void kafka::KafkaMessageConsumer::processMessage(const cppkafka::Message& message) {
    if (message.get_error()) {
        std::cerr << "Error: "
                    << message.get_error().to_string()
                    << std::endl;
        return;
    }
    std::string payload = message.get_payload();
    m_consumer->commit(message);
}

void kafka::execConsumer(const std::string& brokers,
                         const std::string& topic,
                         const std::string& group_id) {
    KafkaMessageConsumer consumer(brokers, topic, group_id);
    consumer.start();
}

void kafka::execAsyncConsumer(const std::string& brokers,
                              const std::string& topic,
                              const std::string& group_id) {
    KafkaMessageConsumer consumer(brokers, topic, group_id);
    std::thread consumerThread([&]() {
        consumer.start();
    });
    consumerThread.join();
}