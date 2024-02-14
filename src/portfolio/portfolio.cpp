#include <iostream>
#include "portfolio.hpp"

bool portfolio::Portfolio::addInvestment(const Investment& investment) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == investment.getTicker()) {
            if (it->getName() != investment.getName()) {
                log("[Portfolio]" + m_name + ": Add - Failure");
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
    log("[Portfolio]" + m_name + ": Add - Success");
    return retVal;
}

bool portfolio::Portfolio::removeInvestment(const std::string& ticker) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == ticker) {
            m_investments.erase(it);
            retVal = true;
            break;
        }
    }
    log("[Portfolio]" + m_name + ": Remove - Success");
    return retVal;
}

void portfolio::Portfolio::clearInvestments() {
    std::vector<std::string> tickers;
    for (const auto& investment : m_investments) {
        tickers.push_back(investment.getTicker());
    }
    for (const auto& ticker : tickers) {
        (void) removeInvestment(ticker);
    }
}

double_t portfolio::Portfolio::calculateTotalValue() const {
    double_t totalValue = 0.0;
    for (const auto& investment : m_investments) {
        totalValue += investment.getPurchasePrice() * investment.getQuantity();
    }
    return totalValue;
}

bool portfolio::Portfolio::updatedInvestmentValue(const std::string& ticker, double_t newValue) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setCurrentPrice(newValue);
            retVal = true;
            break;
        }
    }
    log("[Portfolio]" + m_name + ": Update - Success");
    return retVal;
}

bool portfolio::Portfolio::updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setQuantity(newQuant);
            retVal = true;
            break;
        }
    }
    log("[Portfolio]" + m_name + ": Update - Success");
    return retVal;
}

void portfolio::PortfolioManager::addPortfolio(Portfolio& portfolio) {
    m_portfolios.push_back(std::make_unique<Portfolio>(portfolio));
    if (m_setLogger) {
        for (const auto& portfolio : m_portfolios) {
            portfolio->setLoggerPtr(m_logger);
        }
    }
}

bool portfolio::PortfolioManager::addPortfolio(const std::string& portfolio_name) {
    for (auto it = m_portfolios.begin(); it != m_portfolios.end(); ++it) {
        if (it->get()->getName() == portfolio_name) {
            return false;
        }
    }
    m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    if (m_setLogger) {
        for (const auto& portfolio : m_portfolios) {
            portfolio->setLoggerPtr(m_logger);
        }
    }
    log("[PortfolioManager] Add - Success");
    return true;
}

bool portfolio::PortfolioManager::removePortfolio(const std::string& portfolio_name) {
    bool retVal = false;
    for (auto it = m_portfolios.begin(); it != m_portfolios.end(); ++it) {
        if (it->get()->getName() == portfolio_name) {
            m_portfolios.erase(it);
            retVal = true;
            break;
        }
    }
    if (retVal) {
        log("[PortfolioManager] Remove - Success");
    }
    else {
        log("[PortfolioManager] Remove - Failure");
    }
    return retVal;
}