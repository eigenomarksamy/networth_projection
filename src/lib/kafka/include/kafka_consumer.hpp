#ifndef KAFKA_CONSUMER_HPP_
#define KAFKA_CONSUMER_HPP_

#include <memory>
#include <string>
#include <iostream>
#include <cppkafka/consumer.h>

namespace kafka {

class KafkaMessageConsumer {
public:
    KafkaMessageConsumer(const std::string& brokers,
                         const std::string& topic,
                         const std::string& group_id)
        : m_brokers(brokers), m_topic(topic), m_group_id(group_id) {
        cppkafka::Configuration config = {
            { "metadata.broker.list", brokers },
            { "group.id", group_id },
            { "enable.auto.commit", false}
        };
        m_consumer = std::make_unique<cppkafka::Consumer>(config);
    }

    void start();

private:
    void processMessage(const cppkafka::Message& message);

private:
    std::string m_brokers;
    std::string m_topic;
    std::string m_group_id;
    std::unique_ptr<cppkafka::Consumer> m_consumer;
};

void execConsumer(const std::string& brokers,
                  const std::string& topic,
                  const std::string& group_id);

void execAsyncConsumer(const std::string& brokers,
                       const std::string& topic,
                       const std::string& group_id);

}; // namespace kafka

#endif /* KAFKA_CONSUMER_HPP_ */