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

    void addInvestment(const Investment& investment);
    void removeInvestment(const std::string& ticker);
    void displayPortfolio() const;
    double_t calculateTotalValue() const;
    void updatedInvestmentValue(const std::string& ticker, double_t newValue);
};

class PortfolioManager {
    std::unique_ptr<Portfolio> m_portfolio;
public:
    PortfolioManager(const std::string& portfolio_name) :
        m_portfolio(std::make_unique<Portfolio>(portfolio_name)) {}
    void executeManagement();
};


#endif /* PORTFOLIO_HPP_ */