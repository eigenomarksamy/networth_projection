#include <iostream>
#include <fstream>
#include <sstream>
#include "portfolio.hpp"
#include "input_handler.hpp"

bool Portfolio::addInvestment(const Investment& investment) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == investment.getTicker()) {
            if (it->getName() != investment.getName()) {
                return retVal;
            }
            it->setPurchasePrice(investment.getPurchasePrice());
            it->setQuantity(investment.getQuantity());
            retVal = true;
            break;
        }
    }
    if (!retVal) {
        m_investments.push_back(investment);
        retVal = true;
    }
    return retVal;
}

bool Portfolio::removeInvestment(const std::string& ticker) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == ticker) {
            m_investments.erase(it);
            retVal = true;
            break;
        }
    }
    return retVal;
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

void Portfolio::clearInvestments() {
    std::vector<std::string> tickers;
    for (const auto& investment : m_investments) {
        tickers.push_back(investment.getTicker());
    }
    for (const auto& ticker : tickers) {
        (void) removeInvestment(ticker);
    }
}

double_t Portfolio::calculateTotalValue() const {
    double_t totalValue = 0.0;
    for (const auto& investment : m_investments) {
        totalValue += investment.getPurchasePrice() * investment.getQuantity();
    }
    return totalValue;
}

bool Portfolio::updatedInvestmentValue(const std::string& ticker, double_t newValue) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setPurchasePrice(newValue);
            retVal = true;
            break;
        }
    }
    return retVal;
}

bool Portfolio::updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setQuantity(newQuant);
            retVal = true;
            break;
        }
    }
    return retVal;
}

void PortfolioManager::addPortfolio(Portfolio& portfolio) {
    m_portfolios.push_back(std::make_unique<Portfolio>(portfolio));
}

bool PortfolioManager::addPortfolio(const std::string& portfolio_name) {
    for (auto it = m_portfolios.begin(); it != m_portfolios.end(); ++it) {
        if (it->get()->getName() == portfolio_name) {
            return false;
        }
    }
    m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    return true;
}

bool PortfolioManager::removePortfolio(const std::string& portfolio_name) {
    bool retVal = false;
    for (auto it = m_portfolios.begin(); it != m_portfolios.end(); ++it) {
        if (it->get()->getName() == portfolio_name) {
            m_portfolios.erase(it);
            retVal = true;
            break;
        }
    }
    return retVal;
}

void savePortfolio(const Portfolio& portfolio, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << portfolio.getName() << std::endl;
        const std::vector<Investment>& investments = portfolio.getInvestments();
        for (const Investment& investment : investments) {
            file << investment.getName() << "," << investment.getTicker() << ","
                 << investment.getPurchasePrice() << "," << investment.getQuantity() << std::endl;
        }
        std::cout << "Portfolio saved to " << filename << std::endl;
    }
    else {
        std::cout << "Unable to open file for saving portfolio." << std::endl;
    }
    file.close();
}

bool loadPortfolio(Portfolio& portfolio, const std::string& filename) {
    bool status = true;
    std::ifstream file(filename);
    portfolio.clearInvestments();
    if (file.is_open()) {
        std::string name, ticker, line;
        double_t purchasePrice;
        uint32_t quantity;
        std::getline(file, name);
        portfolio.setName(name);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::getline(iss, name, ',');
            std::getline(iss, ticker, ',');
            iss >> purchasePrice;
            iss.ignore();
            iss >> quantity;
            Investment investment(name, ticker, purchasePrice, quantity);
            portfolio.addInvestment(investment);
        }
        std::cout << "Portfolio loaded from " << filename << std::endl;
    }
    else {
        status = false;
        std::cout << "Unable to open file for load portfolio.\n";
    }
    file.close();
    return status;
}