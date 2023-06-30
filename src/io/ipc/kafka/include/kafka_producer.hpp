#ifndef KAFKA_PRODUCER_HPP_
#define KAFKA_PRODUCER_HPP_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <cppkafka/cppkafka.h>
#include "utils.hpp"

namespace kafka {

enum class ModeOp {
    SINGLE_PRODUCER_SINGLE_TOPIC,
    SINGLE_PRODUCER_MULTI_TOPIC,
    MULTI_TOPIC_MULTI_PRODUCER
};

enum class ModeProducer {
    NONE,
    AUTO,
    LO_PRIO,
    HI_PRIO
};

enum class ErrorCode {
    NONE,
    MAX_COUNT,
    MAX_TOPICS,
    MAX_LOAD_PRODUCER,
    NOT_INITIALIZED,
    COMPUTATION
};

inline size_t getHash(const std::string& brokerConf, const std::string& brokerList) {
    return generateHashForString(brokerConf + brokerList);
}

class KafkaTopic {

public:
    KafkaTopic(const std::string& name)
        : m_name(name), m_max_count(0u) {}
    KafkaTopic(const std::string& name, const uint64_t max_count)
        : m_name(name), m_max_count(max_count) {}

    std::string getName() const { return m_name; }
    uint64_t getCount() const { return m_count; }
    ErrorCode incrementCount();

private:
    std::string m_name;
    uint64_t m_max_count;
    uint64_t m_count;
};

class KafkaProducer {

public:
    KafkaProducer(const std::string& brokerList)
        : m_config{{"metadata.broker.list", brokerList}},
          m_is_single_topic(false),
          m_max_load(0u),
          m_load(0u) {
        m_unique_identifier = getHash("metadata.broker.list", brokerList);
        m_producer = std::make_unique<cppkafka::Producer>(m_config);
    }
    KafkaProducer(const std::string& brokerList,
                  const std::string& brokerConf)
        : m_config{{brokerConf, brokerList}},
          m_is_single_topic(false),
          m_max_load(0u),
          m_load(0u) {
        m_unique_identifier = getHash(brokerConf, brokerList);
        m_producer = std::make_unique<cppkafka::Producer>(m_config);
    }
    KafkaProducer(const std::string& brokerList,
                  const std::string& brokerConf,
                  const bool is_single_topic)
        : m_config{{brokerConf, brokerList}},
        m_is_single_topic(is_single_topic),
        m_max_load(0u),
        m_load(0u) {
        m_unique_identifier = getHash(brokerConf, brokerList);
        m_producer = std::make_unique<cppkafka::Producer>(m_config);
    }
    KafkaProducer(const std::string& brokerList,
                  const std::string& brokerConf,
                  const bool is_single_topic,
                  const uint64_t max_load)
        : m_config{{brokerConf, brokerList}},
        m_is_single_topic(is_single_topic),
        m_max_load(max_load),
        m_load(0u) {
        m_unique_identifier = getHash(brokerConf, brokerList);
        m_producer = std::make_unique<cppkafka::Producer>(m_config);
    }
    ~KafkaProducer() {
        m_producer->flush();
        m_producer.reset(nullptr);
        for (auto& ptr : m_topics) {
            ptr.reset(nullptr);
        }
    }

    uint64_t getLoad() const { return m_load; }

    ErrorCode produce(const std::string& topic, const std::string& message,
                      const uint64_t max_count=0);

    size_t getId() const { return m_unique_identifier; }

private:
    cppkafka::Configuration m_config;
    size_t m_unique_identifier;
    std::unique_ptr<cppkafka::Producer> m_producer;
    std::vector<std::unique_ptr<KafkaTopic>> m_topics;
    uint64_t m_load;
    uint64_t m_max_load;
    bool m_is_single_topic;
};

class KafkaManager {

public:
    KafkaManager() {}
    ~KafkaManager() {
        for (auto& ptr : m_producers) {
            ptr.reset(nullptr);
        }
    }

    ErrorCode produce(const ModeProducer& prod_mode,
                      const std::string& brokerList,
                      const std::string& brokerConf,
                      const std::string& topic,
                      const std::string& message,
                      const uint64_t max_load,
                      const uint64_t max_count);

private:
    std::vector<std::unique_ptr<KafkaProducer>> m_producers;
};

} // namespace kafka

#endif /* KAFKA_PRODUCER_HPP_ */