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
};

class PortfolioManager {
    std::unique_ptr<Portfolio> m_portfolio;
public:
    PortfolioManager(const std::string& portfolio_name) :
        m_portfolio(std::make_unique<Portfolio>(portfolio_name)) {}
    ~PortfolioManager() {
        m_portfolio.reset(nullptr);
    }
    void executeManagement();
};


#endif /* PORTFOLIO_HPP_ */