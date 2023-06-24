#include "kafka_producer.hpp"

kafka::ErrorCode kafka::KafkaTopic::incrementCount() {
    if (m_max_count != 0) {
        return ( ++m_count <= m_max_count ) ? ErrorCode::NONE : ErrorCode::MAX_COUNT;
    }
    else {
        m_count++;
        return ErrorCode::NONE;
    }
}

kafka::ErrorCode kafka::KafkaProducer::produce(const std::string& topic,
                                               const std::string& message,
                                               const uint64_t max_count) {
    if (!m_producer) {
        return ErrorCode::NOT_INITIALIZED;
    }
    bool does_topic_exist = false;
    size_t idx = 0;
    for (const auto& it : m_topics) {
        if (it->getName() == topic) {
            does_topic_exist = true;
            break;
        }
        idx++;
    }
    if (does_topic_exist) {
        if (m_topics[idx]->incrementCount() == ErrorCode::MAX_COUNT) {
            return ErrorCode::MAX_COUNT;
        }
    }
    else if (!m_is_single_topic) {
        m_topics.push_back(std::make_unique<KafkaTopic>(topic, max_count));
        (void)m_topics.back()->incrementCount();
    }
    else {
        return ErrorCode::MAX_TOPICS;
    }
    m_load++;
    if (m_max_load != 0 && m_load >= m_max_load) {
        return ErrorCode::MAX_LOAD_PRODUCER;
    }
    m_producer->produce(cppkafka::MessageBuilder(topic).payload(message));
    return ErrorCode::NONE;
}

kafka::ErrorCode kafka::KafkaManager::produce(const ModeProducer& prod_mode,
                                              const std::string& brokerList,
                                              const std::string& brokerConf,
                                              const std::string& topic,
                                              const std::string& message,
                                              const uint64_t max_load,
                                              const uint64_t max_count) {
    uint64_t idx;
    if (prod_mode == ModeProducer::HI_PRIO) {
        idx = 0;
        for (auto& producer : m_producers) {
            if(producer->getId() == getHash(brokerConf, brokerList)) {
                break;
            }
            idx++;
        }
        if (idx >= m_producers.size()) {
            m_producers.push_back(std::make_unique<KafkaProducer>(brokerList,
                                                                  brokerConf,
                                                                  true,
                                                                  max_load));
            return m_producers[m_producers.size() - 1]->produce(topic, message, max_count);
        }
        else {
            return m_producers[idx]->produce(topic, message, max_count);
        }
    }
    else {
        for (auto& producer : m_producers) {
            if(producer->getId() == getHash(brokerConf, brokerList)) {
                break;
            }
            idx++;
        }
        if (idx >= m_producers.size()) {
            m_producers.push_back(std::make_unique<KafkaProducer>(brokerList,
                                                                  brokerConf,
                                                                  false,
                                                                  max_load));
            return m_producers[m_producers.size() - 1]->produce(topic, message, max_count);
        }
        else {
            if (prod_mode == ModeProducer::LO_PRIO) {
                uint64_t max_load = 0;
                idx = 0;
                for (const auto& it : m_producers) {
                    if (it->getLoad() >= max_load) {
                        max_load = it->getLoad();
                        idx++;
                    }
                }
                return m_producers[--idx]->produce(topic, message, max_count);
            }
            else {
                uint64_t sum_load = 0;
                std::vector<uint64_t> loads;
                for (const auto& it : m_producers) {
                    sum_load += it->getLoad();
                    loads.push_back(it->getLoad());
                }
                uint64_t avg_load = sum_load / m_producers.size();
                auto target_load = findClosest(loads, avg_load);
                auto it = std::find(loads.begin(), loads.end(), target_load);
                if (it != loads.end()) {
                    idx = it - loads.begin();
                    return m_producers[idx]->produce(topic, message, max_count);
                }
                else {
                    return ErrorCode::COMPUTATION;
                }
            }
        }
    }
}