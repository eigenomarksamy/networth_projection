#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

class Logger {
public:

    Logger() : m_filename("dump_log"), m_append(true), m_displayLog(true) {}

    Logger(const std::string& filename)
      : m_filename(filename), m_append(false), m_displayLog(false) {}

    Logger(const std::string& filename, bool append)
      : m_filename(filename), m_append(append), m_displayLog(false) {}

    Logger(const std::string& filename, bool append, bool display)
      : m_filename(filename), m_append(append), m_displayLog(display) {}

    ~Logger() {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    Logger(const Logger& other) = delete;

    Logger(const Logger&& other) noexcept
      : m_filename(std::move(other.m_filename)),
        m_append(other.m_append),
        m_displayLog(other.m_displayLog),
        m_logFile(std::move(other.m_logFile)) {
        other.m_logFile.close();
    }

    enum class LogLevel { None, Event, Info, Warning, Error };

    class FunctionalModules {
    public:
        virtual std::string stringify() = 0;
        virtual void setSubModule(const std::string& moduleName) = 0;
    };

    struct LogLocation {
        std::vector<std::string> callStack;
        std::string fileName;
        std::string lineNumber;
    };

    void log(const std::string& message) const;
    void log(const std::string& message, const LogLevel level) const;
    void log(const std::string& message, const LogLocation& location) const;
    void log(const std::string& message, FunctionalModules& fModule) const;

private:
    std::string m_filename;
    bool m_append;
    bool m_displayLog;
    mutable std::ofstream m_logFile;
};


#endif /* LOGGER_HPP_ */