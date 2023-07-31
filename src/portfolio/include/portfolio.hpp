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
    std::unique_ptr<PortfolioLogger> m_logger;

public:
    Portfolio(const bool log = true) {
        if (log) {
            m_logger = std::make_unique<PortfolioLogger>();
        }
    }
    Portfolio(const std::string& name, const bool log = true) : m_name(name) {
        if (log) {
            m_logger = std::make_unique<PortfolioLogger>();
        }
    }
    Portfolio(const Portfolio& other) {
        m_name = other.m_name;
        m_investments = other.m_investments;
    }
    ~Portfolio() {
        m_logger.reset(nullptr);
    }

    bool addInvestment(const Investment& investment);
    bool removeInvestment(const std::string& ticker);
    bool updatedInvestmentValue(const std::string& ticker, double_t newValue);
    bool updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant);
    double_t calculateTotalValue() const;
    void clearInvestments();
    void setName(const std::string& name) { m_name = name; }
    std::string getName() const { return m_name; }
    std::vector<Investment> getInvestments() const { return m_investments; }
    friend void displayPortfolio(const Portfolio& obj);

private:
    void log(const std::string& msg) const {
        if (m_logger.get()) {
            m_logger->log(msg);
        }
    }
};

void displayPortfolio(const Portfolio& obj);

class PortfolioManager {

    std::vector<std::unique_ptr<Portfolio>> m_portfolios;
    std::unique_ptr<PortfolioLogger> m_logger;

public:

    PortfolioManager(const bool log = true) {
        if (log) {
            m_logger = std::make_unique<PortfolioLogger>();
        }
    }

    PortfolioManager(const std::string& portfolio_name, const bool log = true) {
        m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name, log));
        if (log) {
            m_logger = std::make_unique<PortfolioLogger>();
        }
    }

    PortfolioManager(const std::vector<std::string>& portfolios_names, const bool log = true) {
        for (const auto& portfolio_name : portfolios_names) {
            m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name, log));
        }
        if (log) {
            m_logger = std::make_unique<PortfolioLogger>();
        }
    }

    ~PortfolioManager() {
        for (auto& portfolio_ptr : m_portfolios) {
            portfolio_ptr.reset(nullptr);
        }
        m_logger.reset(nullptr);
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

private:
    void log(const std::string& msg) const {
        if (m_logger.get()) {
            m_logger->log(msg);
        }
    }

};

} // namespace portfolio

#endif /* PORTFOLIO_HPP_ */