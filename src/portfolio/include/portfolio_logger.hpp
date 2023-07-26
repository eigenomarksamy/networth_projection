#ifndef PORTFOLIO_LOGGER_HPP_
#define PORTFOLIO_LOGGER_HPP_

#include <string>
#include <memory>
#include "utils.hpp"
#include "logger.hpp"

namespace portfolio {

class PortfolioLogger {

public:

    PortfolioLogger() {
        m_logger = std::make_unique<Logger>("gen/logs/log_prtfmgr_" + getUniqueTimeId() + ".log");
        m_seq = 0u;
    }

    ~PortfolioLogger() {
        m_logger.reset(nullptr);
    }

    void log(const std::string& message) {
        m_logger->Log(getUniqueTimeId() + "_" + std::to_string(m_seq) + ": " + message);
        ++m_seq;
    }

private:
    std::unique_ptr<Logger> m_logger;
    uint64_t m_seq;
};

} // namespace portfolio

#endif /* PORTFOLIO_LOGGER_HPP_ */