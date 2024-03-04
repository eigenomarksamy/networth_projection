#include <iostream>
#include "portfolio.hpp"

bool portfolio::Portfolio::addInvestment(const Investment& investment) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == investment.getTicker()) {
            if (it->getName() != investment.getName()) {
                // log(m_name, "Add", retVal);
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
    // log(m_name, "Add", retVal);
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
    // log(m_name, "Remove", retVal);
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
        totalValue += investment.getCurrentPrice() * investment.getQuantity();
    }
    return totalValue;
}

double_t portfolio::Portfolio::calculateTotalGain() const {
    double_t totalGain = 0.0;
    for (const auto& investment : m_investments) {
        totalGain += (investment.getCurrentPrice() - investment.getPurchasePrice()) * investment.getQuantity();
    }
    return totalGain;
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
    // log(m_name, "Update", retVal);
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
    // log(m_name, "Update", retVal);
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
            // log("Add", false);
            return false;
        }
    }
    m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    if (m_setLogger) {
        for (const auto& portfolio : m_portfolios) {
            portfolio->setLoggerPtr(m_logger);
        }
    }
    // log("Add", true);
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
    // log("Remove", retVal);
    return retVal;
}

ComplexInvestment portfolio::TransactionalPortfolio::createComplexInvestment(const Investment& investment,
                                                                             const double_t fees) {
    ComplexInvestment complex_investment;
    complex_investment.setInvestment(investment);
    Transaction transaction;
    DateTime date = DateTime::getDateTimeNow();
    transaction.m_datetime = date;
    transaction.m_fees = fees;
    complex_investment.setTransaction(transaction);
    complex_investment.setId(std::to_string(generateHashForString(investment.getName() + investment.getTicker() + std::to_string(investment.getPurchasePrice()) + DateTime::getUniqueTimeId())));
    return complex_investment;
}

ComplexInvestment portfolio::TransactionalPortfolio::createComplexInvestment(
                    const Investment& investment,
                    const DateTime& date,
                    const double_t fees,
                    const Transaction::Currency currency,
                    const uint32_t sequencer,
                    const double_t currency_conv_rate,
                    const double_t currency_conv_fees) {
    ComplexInvestment complex_investment;
    complex_investment.setInvestment(investment);
    Transaction transaction;
    transaction.m_datetime = date;
    transaction.m_currency = currency;
    transaction.m_fees = fees;
    transaction.m_conversion_fees = currency_conv_fees;
    complex_investment.setTransaction(transaction);
    complex_investment.setId(std::to_string(generateHashForString(investment.getName() +
                                                                  investment.getTicker() +
                                                                  DateTime::getUniqueTimeId())));
    complex_investment.setSequencer(sequencer);
    complex_investment.setCurrencyConversionRate(currency_conv_rate);
    return complex_investment;
}

void portfolio::TransactionalPortfolio::updateSequence() {
    uint32_t i = 0;
    for (auto& investment : m_investments) {
        investment.setSequencer(i);
        ++i;
    }
}

bool portfolio::TransactionalPortfolio::addInvestments(const std::vector<ComplexInvestment>& investments) {
    bool retVal = true;
    for (const auto& investment : investments) {
        if (std::find(m_investments.begin(), m_investments.end(), investment) == m_investments.end()) {
            m_investments.push_back(investment);
            retVal &= true;
        }
        else {
            retVal = false;
        }
    }
    return retVal;
}

bool portfolio::TransactionalPortfolio::removeInvestments(const std::vector<std::string>& investmentIds) {
    bool retVal = true;
    for (const auto& investmentId : investmentIds) {
        auto it = std::find_if(m_investments.begin(), m_investments.end(),
                                [&](const auto& stored_investment) {
                                    return stored_investment.getId() == investmentId;
                                });
        if (it != m_investments.end()) {
            m_investments.erase(it);
            retVal &= true;
        }
        else {
            retVal = false;
        }
    }
    return retVal;
}

bool portfolio::TransactionalPortfolio::updateInvestments(const std::map<std::string, std::pair<double_t, InvestmentParameters>>& mapOfUpdate) {
    bool retVal = true;
    for (auto it = mapOfUpdate.begin(); it != mapOfUpdate.end(); ++it) {
        auto id = it->first;
        auto value = it->second.first;
        auto param = it->second.second;
        auto check_it = std::find_if(m_investments.begin(), m_investments.end(),
                                        [&](const auto& stored_investment) {
                                        return stored_investment.getId() == id;
                                        });
        if (param == InvestmentParameters::PurchasePrice) {
            if (check_it != m_investments.end()) {
                check_it->setInvestment(Investment(check_it->getInvestment().getName(),
                                                   check_it->getInvestment().getTicker(),
                                                   value,
                                                   check_it->getInvestment().getCurrentPrice(),
                                                   check_it->getInvestment().getQuantity()));
            }
            else {
                retVal = false;
            }
        }
        else if (param == InvestmentParameters::CurrentPrice) {
            if (check_it != m_investments.end()) {
                check_it->setInvestment(Investment(check_it->getInvestment().getName(),
                                                   check_it->getInvestment().getTicker(),
                                                   check_it->getInvestment().getPurchasePrice(),
                                                   value,
                                                   check_it->getInvestment().getQuantity()));
            }
            else {
                retVal = false;
            }
        }
        else {
            if (check_it != m_investments.end()) {
                check_it->setInvestment(Investment(check_it->getInvestment().getName(),
                                                   check_it->getInvestment().getTicker(),
                                                   check_it->getInvestment().getPurchasePrice(),
                                                   check_it->getInvestment().getCurrentPrice(),
                                                   static_cast<uint32_t>(value)));
            }
            else {
                retVal = false;
            }
        }
    }
    return retVal;
}

double_t portfolio::TransactionalPortfolio::calculateTotalPurchases() const {
    double_t retVal = 0.0;
    for (const auto& investment : m_investments) {
        retVal += (investment.getInvestment().getQuantity() * investment.getInvestment().getPurchasePrice());
    }
    return retVal;
}

double_t portfolio::TransactionalPortfolio::calculateTotalValue() const {
    double_t retVal = 0.0;
    for (const auto& investment : m_investments) {
        retVal += (investment.getInvestment().getQuantity() * investment.getInvestment().getCurrentPrice());
    }
    return retVal;
}

double_t portfolio::TransactionalPortfolio::calculateTotalGain() const {
    double_t retVal = calculateTotalValue() - calculateTotalPurchases();
    for (const auto& investment : m_investments) {
        retVal -= investment.getTransaction().m_fees;
    }
    return retVal;
}

std::vector<ComplexInvestment> portfolio::TransactionalPortfolio::getFilteredDateInvestments(const DateTime& datetime) const {
    std::vector<ComplexInvestment> retVec;
    for (const auto& investment : m_investments) {
        if (investment.getTransaction().m_datetime == datetime) {
            retVec.push_back(investment);
        }
    }
    return retVec;
}

std::vector<ComplexInvestment> portfolio::TransactionalPortfolio::getFilteredSymbolInvestments(const std::string& ticker) const {
    std::vector<ComplexInvestment> retVec;
    for (const auto& investment : m_investments) {
        if (investment.getInvestment().getTicker() == ticker) {
            retVec.push_back(investment);
        }
    }
    return retVec;
}

void portfolio::TransactionalPortfolioManager::addPortfolio(TransactionalPortfolio& portfolio) {
    m_portfolios.push_back(std::make_unique<TransactionalPortfolio>(portfolio));
}

bool portfolio::TransactionalPortfolioManager::addPortfolio(const std::string& portfolio_name) {
    for (auto it = m_portfolios.begin(); it != m_portfolios.end(); ++it) {
        if (it->get()->getName() == portfolio_name) {
            return false;
        }
    }
    m_portfolios.push_back(std::make_unique<TransactionalPortfolio>(portfolio_name));
    return true;
}

bool portfolio::TransactionalPortfolioManager::removePortfolio(const std::string& portfolio_name) {
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
