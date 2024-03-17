#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <ctime>
#include <vector>

class Logger {
public:
    enum class LogLevel { None, Event, Info, Debug, Warning, Error };

    static Logger& instance() {
        static Logger loggerInstance;
        return loggerInstance;
    }

    template <typename T>
    void log(const T& message, const std::vector<std::string>& callStack,
             const std::string& fileName, uint32_t lineNumber) {
        std::lock_guard<std::mutex> lock(mutex);

        std::string logTime = getLogTime();
        std::cout << "[" << logTime << "] ";
        if (!callStack.empty()) {
            std::cout << "Call Stack: ";
            for (const auto& func : callStack) {
                std::cout << func << " ";
            }
            std::cout << "| ";
        }
        std::cout << "File: " << fileName << " | Line: " << lineNumber << " | ";
        std::cout << "LOG: " << message << std::endl;

        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] ";
            if (!callStack.empty()) {
                m_logFile << "Call Stack: ";
                for (const auto& func : callStack) {
                    m_logFile << func << " ";
                }
                m_logFile << "| ";
            }
            m_logFile << "File: " << fileName << " | Line: " << lineNumber << " | ";
            m_logFile << "LOG: " << message << std::endl;
            m_logFile.flush();
        }
    }

    template <typename T>
    void log(const T& message, const std::vector<std::string>& callStack) {
        std::lock_guard<std::mutex> lock(mutex);

        std::string logTime = getLogTime();
        std::cout << "[" << logTime << "] ";
        if (!callStack.empty()) {
            std::cout << "Call Stack: ";
            for (const auto& func : callStack) {
                std::cout << func << " ";
            }
            std::cout << "| ";
        }
        std::cout << "LOG: " << message << std::endl;

        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] ";
            if (!callStack.empty()) {
                m_logFile << "Call Stack: ";
                for (const auto& func : callStack) {
                    m_logFile << func << " ";
                }
                m_logFile << "| ";
            }
            m_logFile << "LOG: " << message << std::endl;
            m_logFile.flush();
        }
    }

    template <typename T>
    void log(const T& message, const std::string& fileName, uint32_t lineNumber) {
        std::lock_guard<std::mutex> lock(mutex);

        std::string logTime = getLogTime();
        std::cout << "[" << logTime << "] ";
        std::cout << "File: " << fileName << " | Line: " << lineNumber << " | ";
        std::cout << "LOG: " << message << std::endl;

        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] ";
            m_logFile << "File: " << fileName << " | Line: " << lineNumber << " | ";
            m_logFile << "LOG: " << message << std::endl;
            m_logFile.flush();
        }
    }

    template <typename T>
    void log(const T& message, const LogLevel logLevel) {
        std::lock_guard<std::mutex> lock(mutex);

        std::string logTime = getLogTime();
        std::cout << "[" << logTime << "] ";
        std::cout << "[ " + getLogLevel(logLevel) + " ] ";
        std::cout << "LOG: " << message << std::endl;

        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] ";
            m_logFile << "[ " + getLogLevel(logLevel) + " ] ";
            m_logFile << "LOG: " << message << std::endl;
            m_logFile.flush();
        }
    }

    template <typename T>
    void log(const T& message) {
        std::lock_guard<std::mutex> lock(mutex);

        std::string logTime = getLogTime();
        std::cout << "[" << logTime << "] " << "LOG: " << message << std::endl;
        if (m_logFile.is_open()) {
            m_logFile << "[" << logTime << "] " << "LOG: " << message << std::endl;
            m_logFile.flush();
        }
    }

    void setLogFile(const std::string& filename, const bool append) {
        std::lock_guard<std::mutex> lock(mutex);

        m_logFile.open(filename, append ? std::ios::out | std::ios::app : std::ios::out);
        if (!m_logFile) {
            std::cerr << "Error opening log file: " << filename << std::endl;
        }
    }

    ~Logger() {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

private:
    Logger() {}

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getLogTime() const {
        std::time_t currentTime = std::time(nullptr);
        std::string logTime = std::asctime(std::localtime(&currentTime));
        logTime.pop_back();
        return logTime;
    }

    std::string getLogLevel(const LogLevel logLevel) const {
        std::string log_level;
        switch (logLevel) {
        case LogLevel::Event:
            log_level = "EVENT";
            break;
        case LogLevel::Info:
            log_level = "INFO";
            break;
        case LogLevel::Debug:
            log_level = "DEBUG";
            break;
        case LogLevel::Warning:
            log_level = "WARNING";
            break;
        case LogLevel::Error:
            log_level = "ERROR";
            break;
        default:
            log_level = "";
            break;
        }
        return log_level;
    }

    std::ofstream m_logFile;
    std::mutex mutex;
};



    // Example usage:
    // Logger::instance().log("This is a log message.", {"func1", "func2"}, __FILE__, __LINE__);
    // Logger::instance().setLogFile("log.txt");

    // Example usage:
    // Logger::instance().log("This is a log message.", {"func1", "func2"});
    // Logger::instance().setLogFile("log.txt");

    // Example usage:
    // Logger::instance().log("This is a log message.", __FILE__, __LINE__);
    // Logger::instance().setLogFile("log.txt");

    // Example usage:
    // Logger::instance().log("This is a log message.", Logger::LogLevel::Info);
    // Logger::instance().setLogFile("log.txt");

    // Example usage:
    // Logger::instance().log("This is a log message.");
    // Logger::instance().setLogFile("log.txt");

#endif /* LOGGER_HPP_ */
