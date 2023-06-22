#ifndef KAFKA_WRAPPER_HPP_
#define KAFKA_WRAPPER_HPP_

#include <string>
#include <unordered_map>
#include <cppkafka/cppkafka.h>

class KafkaWrapper {
public:
    KafkaWrapper(const std::string& brokerList)
        : m_config{{"metadata.broker.list", brokerList}} {
        m_producer = std::make_unique<cppkafka::Producer>(m_config);
    }
    ~KafkaWrapper() {
        m_producer->flush();
        m_producer.reset(nullptr);
    }

    void produce(const std::string& topic, const std::string& message) {
        m_producer->produce(cppkafka::MessageBuilder(topic).payload(message));
    }

private:
    cppkafka::Configuration m_config;
    std::unique_ptr<cppkafka::Producer> m_producer;
};


#endif /* KAFKA_WRAPPER_HPP_ */