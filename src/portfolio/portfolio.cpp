#include <iostream>
#include "portfolio.hpp"

void Portfolio::addInvestment(const Investment& investment) {
    m_investments.push_back(investment);
}

void Portfolio::removeInvestment(const std::string& ticker) {
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == ticker) {
            m_investments.erase(it);
            break;
        }
    }
}

void Portfolio::displayPortfolio() const {
    std::cout << "Portfolio: " << m_name << std::endl;
    for (const auto& investment : m_investments) {
        std::cout << "Name: " << investment.getName() << ", Ticker: "
                  << investment.getTicker() << ", Quantity: "
                  << investment.getQuantity() << ", Purchase price: "
                  << investment.getPurchasePrice()
                  << std::endl;
    }
}

double_t Portfolio::calculateTotalValue() const {
    double_t totalValue = 0.0;
    for (const auto& investment : m_investments) {
        totalValue += investment.getPurchasePrice() * investment.getQuantity();
    }
    return totalValue;
}

void Portfolio::updatedInvestmentValue(const std::string& ticker, double_t newValue) {
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setPurchasePrice(newValue);
            break;
        }
    }
}