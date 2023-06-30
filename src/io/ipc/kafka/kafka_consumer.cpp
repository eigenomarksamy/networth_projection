#include <iostream>
#include "kafka_consumer.hpp"


kafka::KafkaMessageConsumer::KafkaMessageConsumer(const std::string& brokers,
                        const std::string& topic,
                        const std::string& group_id)
    : m_brokers(brokers), m_topic(topic),
      m_group_id(group_id), m_running(false) {
    cppkafka::Configuration config = {
        { "metadata.broker.list", brokers },
        { "group.id", group_id },
        { "enable.auto.commit", false}
    };
    m_consumer = std::make_unique<cppkafka::Consumer>(config);
}

void kafka::KafkaMessageConsumer::start() {
    if (!m_running) {
        m_running = true;
        m_thread = std::thread(&KafkaMessageConsumer::consumeMessage, this);
    }
}

void kafka::KafkaMessageConsumer::stop() {
    if (m_running) {
        m_running = false;
        m_thread.join();
    }
}

void kafka::KafkaMessageConsumer::consumeMessage() {
    m_consumer->subscribe({ m_topic });
    while (m_running) {
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
    std::cout << payload << std::endl;
    m_consumer->commit(message);
}