#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

#include <string>
#include <vector>
#include <memory>
#include "investment.hpp"
#include "portfolio_logger.hpp"

namespace portfolio {

class Portfolio {
private:
    std::string m_name;
    std::vector<Investment> m_investments;
    std::shared_ptr<PortfolioLogger> m_logger;
    bool m_setLogger;

public:
    Portfolio() {}
    Portfolio(const std::string& name) : m_name(name), m_setLogger(false) {}
    Portfolio(const Portfolio& other) {
        m_name = other.m_name;
        m_investments = other.m_investments;
    }
    ~Portfolio() {}

    bool addInvestment(const Investment& investment);
    bool removeInvestment(const std::string& ticker);
    bool updatedInvestmentValue(const std::string& ticker, double_t newValue);
    bool updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant);
    double_t calculateTotalValue() const;
    void clearInvestments();
    void setName(const std::string& name) { m_name = name; }
    std::string getName() const { return m_name; }
    std::vector<Investment> getInvestments() const { return m_investments; }
    void setLoggerPtr(const std::shared_ptr<PortfolioLogger>& logger) {
        m_logger = logger;
        m_setLogger = true;
    }
    friend void displayPortfolio(const Portfolio& obj);

private:
    void log(const std::string& msg) const {
        if (m_setLogger) {
            m_logger->log(msg);
        }
    }
};

void displayPortfolio(const Portfolio& obj);

class PortfolioManager {

    std::vector<std::unique_ptr<Portfolio>> m_portfolios;
    std::shared_ptr<PortfolioLogger> m_logger;
    bool m_setLogger;

public:

    PortfolioManager() {}

    PortfolioManager(const std::string& portfolio_name) : m_setLogger(false) {
        m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    }

    PortfolioManager(const std::vector<std::string>& portfolios_names)
      : m_setLogger(false) {
        for (const auto& portfolio_name : portfolios_names) {
            m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
        }
    }

    ~PortfolioManager() {
        for (auto& portfolio_ptr : m_portfolios) {
            portfolio_ptr.reset(nullptr);
        }
    }

    void addPortfolio(Portfolio& portfolio);

    bool addPortfolio(const std::string& portfolio_name);

    bool removePortfolio(const std::string& portfolio_name);

    uint16_t getNumPortfolios() const {
        return static_cast<uint16_t>(m_portfolios.size());
    }

    Portfolio& getPortfolio(uint16_t portfolio_idx) const {
        return *(m_portfolios[portfolio_idx]);
    }

    void setLoggerPtr(const std::shared_ptr<PortfolioLogger>& logger) {
        m_logger = logger;
        m_setLogger = true;
    }

private:
    void log(const std::string& msg) const {
        if (m_setLogger) {
            m_logger->log(msg);
        }
    }

};

} // namespace portfolio

#endif /* PORTFOLIO_HPP_ */