#ifndef KAFKA_PRODUCER_HPP_
#define KAFKA_PRODUCER_HPP_

#include <string>
#include <unordered_map>
#include <librdkafka/rdkafkacpp.h>

class KafkaProducer {
public:
    KafkaProducer(const std::string& bootstrapServers);
    ~KafkaProducer();

    bool initialize();
    bool produce(const std::string& topic, const std::string& msg);

private:
    std::string m_bootstrapServers;
    RdKafka::Producer* m_producer;
    std::unordered_map<std::string, RdKafka::Topic*> m_topics;

    void cleanup();
};


#endif /* KAFKA_PRODUCER_HPP_ */