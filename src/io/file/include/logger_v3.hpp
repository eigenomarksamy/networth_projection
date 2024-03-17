#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <string>
#include <memory>

class LoggerBase {
public:
    virtual ~LoggerBase() = default;
    virtual void log(const std::string& message) = 0;
};

class BasicLogger : public LoggerBase {
public:
    void log(const std::string& message) override {
        std::cout << "LOG: " << message << std::endl;
    }
};

class PrefixLoggerDecorator : public LoggerBase {
public:
    PrefixLoggerDecorator(std::unique_ptr<LoggerBase> logger, const std::string& prefix)
        : m_logger(std::move(logger)), m_prefix(prefix) {}

    void log(const std::string& message) override {
        m_logger->log(m_prefix + message);
    }

private:
    std::unique_ptr<LoggerBase> m_logger;
    std::string m_prefix;
};

// // Example usage:
// int main() {
//     // Create a basic logger
//     std::unique_ptr<LoggerBase> basicLogger = std::make_unique<BasicLogger>();

//     // Wrap the basic logger with PrefixLoggerDecorator
//     std::unique_ptr<LoggerBase> decoratedLogger = std::make_unique<PrefixLoggerDecorator>(
//         std::move(basicLogger), "[INFO] ");

//     // Log messages using the decorated logger
//     decoratedLogger->log("This is a decorated log message.");
//     decoratedLogger->log("Another decorated log message.");

//     return 0;
// }

#endif /* LOGGER_HPP_ */