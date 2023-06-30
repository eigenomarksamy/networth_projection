#ifndef KAFKA_CONSUMER_HPP_
#define KAFKA_CONSUMER_HPP_

#include <memory>
#include <string>
#include <atomic>
#include <thread>
#include <cppkafka/consumer.h>

namespace kafka {

class KafkaMessageConsumer {
public:
    KafkaMessageConsumer(const std::string& brokers,
                         const std::string& topic,
                         const std::string& group_id);

    void start();

    void stop();

private:

    void consumeMessage();

    void processMessage(const cppkafka::Message& message);

private:
    std::string m_brokers;
    std::string m_topic;
    std::string m_group_id;
    std::unique_ptr<cppkafka::Consumer> m_consumer;
    std::thread m_thread;
    std::atomic<bool> m_running;
};

}; // namespace kafka

#endif /* KAFKA_CONSUMER_HPP_ */