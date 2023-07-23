#include "logger.hpp"

Logger::Logger(const std::string& filename, bool append) : m_filename(filename) {
    m_logFile.open(filename, append ? std::ios::out | std::ios::app : std::ios::out);
    if (!m_logFile) {
        std::cerr << "Error opening log file: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

/*
 * Sample logger usage:
 *
 * Logger logger("gen/logs/log_" + getUniqueTimeId() + ".log");
 *
 */