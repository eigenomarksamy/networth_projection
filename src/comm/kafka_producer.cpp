#include "kafka_producer.hpp"

KafkaProducer::KafkaProducer(const std::string& bootstrapServers)
    : m_bootstrapServers(bootstrapServers),
      m_producer(nullptr) {}

KafkaProducer::~KafkaProducer() {
    cleanup();
}

bool KafkaProducer::initialize() {
    std::string errStr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    if (!conf) {
        return false;
    }
    conf->set("bootstrap.servers", m_bootstrapServers, errStr);
    if (!errStr.empty()) {
        delete conf;
        return false;
    }
    m_producer = RdKafka::Producer::create(conf, errStr);
    delete conf;
    if (!m_producer) {
        return false;
    }
    return true;
}

bool KafkaProducer::produce(const std::string& topic, const std::string& msg) {
    if (!m_producer) {
        return false;
    }
    RdKafka::Topic* kafkaTopic = nullptr;
    auto it = m_topics.find(topic);
    if (it != m_topics.end()) {
        kafkaTopic = it->second;
    }
    else {
        std::string errStr;
        kafkaTopic = RdKafka::Topic::create(m_producer, topic, nullptr, errStr);
        if (!kafkaTopic) {
            return false;
        }
        m_topics.insert({topic, kafkaTopic});
    }
    RdKafka::ErrorCode resp = m_producer->produce(kafkaTopic,
                                                  RdKafka::Topic::PARTITION_UA,
                                                  RdKafka::Producer::RK_MSG_COPY,
                                                  const_cast<char*>(msg.c_str()),
                                                  msg.size(),
                                                  nullptr, nullptr);
    if (resp != RdKafka::ERR_NO_ERROR) {
        return false;
    }
    m_producer->poll(0);
    return true;
}

void KafkaProducer::cleanup() {
    for (auto& pair : m_topics) {
        delete pair.second;
    }
    m_topics.clear();
    if (m_producer) {
        delete m_producer;
        m_producer = nullptr;
    }
}