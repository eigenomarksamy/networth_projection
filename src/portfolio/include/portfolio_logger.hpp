#ifndef PORTFOLIO_LOGGER_HPP_
#define PORTFOLIO_LOGGER_HPP_

#include <string>
#include <memory>
#include "utils.hpp"
#include "datetime.hpp"
#include "logger.hpp"

namespace portfolio {

class PortfolioLogger {

public:

    class PortfolioModule : public Logger::FunctionalModules {
        std::string m_module = "Portfolio";
        std::vector<std::string> m_subModules;
    public:
        std::string stringify() override {
            return m_module;
        }
        void setSubModule(const std::string& subModuleName) override {
            if (std::find(m_subModules.begin(), m_subModules.end(), subModuleName) != m_subModules.end())
                m_subModules.push_back(subModuleName);
        }
    };

    PortfolioLogger(const std::string& logDir)
     :  m_logger(Logger(logDir + DateTime::getUniqueTimeId() + ".log")) {
        m_fModule = PortfolioModule();
        m_seq = 0u;
    }

    void log(const std::string& message) {
        m_logger.log(DateTime::getUniqueTimeId() + "_" + std::to_string(m_seq) + ": " + message + ".");
        ++m_seq;
    }

    void log(const std::string& message, const Logger::LogLevel level) {
        m_logger.log(DateTime::getUniqueTimeId() + "_" + std::to_string(m_seq) + ": " + message + ".", level);
        ++m_seq;
    }

    void log(const std::string& message, const Logger::LogLocation& location) {
        m_logger.log(DateTime::getUniqueTimeId() + "_" + std::to_string(m_seq) + ": " + message + ".", location);
        ++m_seq;
    }

    void log(const std::string& message, const std::string& submodule) {
        m_logger.log(DateTime::getUniqueTimeId() + "_" + std::to_string(m_seq) + ": " + message + ".", m_fModule);
    }

private:
    Logger m_logger;
    uint64_t m_seq;
    PortfolioModule m_fModule;
};

} // namespace portfolio

#endif /* PORTFOLIO_LOGGER_HPP_ */