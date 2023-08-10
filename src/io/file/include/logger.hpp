#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& filename, bool append = false);

    ~Logger();

    template <typename T>
    void Log(const T& message, LogLevel level = LogLevel::INFO) {
        std::string logLevelStr;
        switch (level) {
            case LogLevel::INFO:
                logLevelStr = "INFO";
                break;
            case LogLevel::WARNING:
                logLevelStr = "WARNING";
                break;
            case LogLevel::ERROR:
                logLevelStr = "ERROR";
                break;
        }

        std::time_t currentTime = std::time(nullptr);
        std::string logTime = std::asctime(std::localtime(&currentTime));
        logTime.pop_back();

        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] " << logLevelStr << ": " << message << std::endl;
            m_logFile.flush();
        }
        std::cout << "[" << logTime << "] " << logLevelStr << ": " << message << std::endl;
    }

private:
    std::string m_filename;
    std::ofstream m_logFile;
};


#endif /* LOGGER_HPP_ */