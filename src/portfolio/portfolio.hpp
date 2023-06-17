#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

#include <string>
#include <vector>
#include <memory>
#include "investment.hpp"

class Portfolio {
private:
    std::string m_name;
    std::vector<Investment> m_investments;

public:
    Portfolio(const std::string& name) : m_name(name) {}

    bool addInvestment(const Investment& investment);
    bool removeInvestment(const std::string& ticker);
    bool updatedInvestmentValue(const std::string& ticker, double_t newValue);
    bool updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant);
    double_t calculateTotalValue() const;
    void displayPortfolio() const;
    std::string getName() const { return m_name; }
};

class PortfolioManager {

    std::vector<std::unique_ptr<Portfolio>> m_portfolios;

public:

    PortfolioManager(const std::string& portfolio_name) {
        m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    }

    PortfolioManager(const std::vector<std::string>& portfolios_names) {
        for (const auto& portfolio_name : portfolios_names) {
            m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
        }
    }

    ~PortfolioManager() {
        for (auto& portfolio_ptr : m_portfolios) {
            portfolio_ptr.reset(nullptr);
        }
    }

    uint16_t getNumPortfolios() const {
        return static_cast<uint16_t>(m_portfolios.size());
    }

    Portfolio& getPortfolio(uint16_t portfolio_idx) const {
        return *(m_portfolios[portfolio_idx]);
    }

};

#endif /* PORTFOLIO_HPP_ */