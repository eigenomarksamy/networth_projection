#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

#include <string>
#include <vector>
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


#endif /* PORTFOLIO_HPP_ */