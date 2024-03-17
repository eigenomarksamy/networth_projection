#include "logger.hpp"

void Logger::log(const std::string& message) const {
    std::time_t currentTime = std::time(nullptr);
    std::string logTime = std::asctime(std::localtime(&currentTime));
    logTime.pop_back();
    m_logFile.open(m_filename, m_append ? std::ios::out | std::ios::app : std::ios::out);
    if (!m_logFile) {
        std::cerr << "Error opening log file: " << m_filename << std::endl;
    }
    else {
        m_logFile << "[" << logTime << "] " << ": " << message << std::endl;
        m_logFile.flush();
        if (m_displayLog)
            std::cout << "[" << logTime << "] " << ": " << message << std::endl;
    }
}

void Logger::log(const std::string& message, const LogLevel level) const {
    std::string logLevelStr;
    switch (level) {
        case LogLevel::Event:
            logLevelStr = "EVENT";
            break;
        case LogLevel::Info:
            logLevelStr = "INFO";
            break;
        case LogLevel::Warning:
            logLevelStr = "WARNING";
            break;
        case LogLevel::Error:
            logLevelStr = "ERROR";
            break;
    }
    std::string log_msg = "[" + logLevelStr + "]" + ": " + message;
    log(log_msg);
}

void Logger::log(const std::string& message, const LogLocation& location) const {
    std::string call_stack_str = "";
    for (const auto& call : location.callStack) {
        call_stack_str += call + " ";
    }
    std::string log_msg = "Log Location:\n\t\tfile: " + location.fileName + "\n"
                          + "\t\tline: " + location.lineNumber + "\n"
                          + "\t\t callstack: " + call_stack_str + "\n"
                          + "message: " + message + "\n";
    log(log_msg);
}

void Logger::log(const std::string& message, FunctionalModules& fModule) const {
    std::string log_msg = fModule.stringify() + ": " + message;
    log(log_msg);
}
