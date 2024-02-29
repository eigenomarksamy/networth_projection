#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

#include <string>
#include <vector>
#include <memory>
#include "investment.hpp"
#include "portfolio_logger.hpp"
#include "portfolio_db.hpp"

namespace portfolio {

class Portfolio {
private:
    std::string m_name;
    std::vector<Investment> m_investments;
    std::shared_ptr<PortfolioLogger> m_logger;
    bool m_setLogger = false;

public:
    Portfolio() {}
    Portfolio(const std::string& name) : m_name(name), m_setLogger(false) {}
    Portfolio(const Portfolio& other)
      : m_name(other.m_name), m_investments(other.m_investments), m_setLogger(other.m_setLogger) {
        if (m_setLogger)
            m_logger = other.m_logger;
    }
    bool operator==(const Portfolio& other) const {
        bool retVal = (((m_setLogger) && (m_setLogger == other.m_setLogger && m_logger == other.m_logger))
                      || ((!m_setLogger) && (m_setLogger == other.m_setLogger)));
        retVal &= (m_name == other.m_name) &&
                  (m_investments == other.m_investments);
        return retVal;
    }
    bool operator!=(const Portfolio& other) const {
        return !(*this == other);
    }
    ~Portfolio() {}

    bool addInvestment(const Investment& investment);
    bool addInvestments(const std::vector<Investment>& investments) {
        m_investments = investments;
        return true;
    }
    bool removeInvestment(const std::string& ticker);
    bool updatedInvestmentValue(const std::string& ticker, double_t newValue);
    bool updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant);
    double_t calculateTotalValue() const;
    double_t calculateTotalGain() const;
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
    void log(const std::string& name, const std::string& operation,
             const bool status) const {
        std::string strStatus = (status) ? "Success" : "Failure";
        std::string msg = "[Portfolio] " + name + ": " + operation + " - " + strStatus;
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

    PortfolioManager() = default;

    PortfolioManager(const std::string& portfolio_name, DatabaseInterface& dbInterface)
      : m_setLogger(false) {
        m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
    }

    PortfolioManager(const std::vector<std::string>& portfolios_names,
                     DatabaseInterface& dbInterface)
      : m_setLogger(false) {
        for (const auto& portfolio_name : portfolios_names) {
            m_portfolios.push_back(std::make_unique<Portfolio>(portfolio_name));
        }
    }

    PortfolioManager(const PortfolioManager& other)
        : m_setLogger(other.m_setLogger) {
        for (const auto& portfolio : other.m_portfolios) {
            m_portfolios.push_back(std::make_unique<Portfolio>(*portfolio));
        }
        if (m_setLogger)
            m_logger = other.m_logger;
    }

    bool operator==(const PortfolioManager& other) const {
        bool retVal = (((m_setLogger) && (m_setLogger == other.m_setLogger && m_logger == other.m_logger))
                      || ((!m_setLogger) && (m_setLogger == other.m_setLogger)));
        retVal &= (m_portfolios == other.m_portfolios);
        return retVal;
    }

    bool operator!=(const PortfolioManager& other) const {
        return !(*this == other);
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
        for (const auto& portfolio : m_portfolios) {
            portfolio->setLoggerPtr(m_logger);
        }
    }

private:
    void log(const std::string& operation, const bool status) const {
        std::string strStatus = (status) ? "Success" : "Failure";
        std::string msg = "[PortfolioManager]: " + operation + " - " + strStatus;
        if (m_setLogger) {
            m_logger->log(msg);
        }
    }

};

class TransactionalPortfolio {

    std::string m_name;
    std::vector<ComplexInvestment> m_investments;

public:
    enum class InvestmentParameters { Quantity, PurchasePrice, CurrentPrice };
    TransactionalPortfolio() = default;
    TransactionalPortfolio(const std::string& name) : m_name(name) {}
    bool operator==(const TransactionalPortfolio& other) const {
        return ((m_name == other.m_name) &&
                (m_investments == other.m_investments));
    }
    bool operator!=(const TransactionalPortfolio& other) const {
        return !(*this == other);
    }
    static ComplexInvestment createComplexInvestment(const Investment& investment, const double_t fees);
    bool addInvestments(const std::vector<ComplexInvestment>& investments);
    bool removeInvestments(const std::vector<std::string>& investmentIds);
    bool updateInvestments(const std::map<std::string, std::pair<double_t, InvestmentParameters>>& mapOfUpdate);
    double_t calculateTotalPurchases() const;
    double_t calculateTotalValue() const;
    double_t calculateTotalGain() const;
    void setName(const std::string& name) { m_name = name; }
    std::string getName() const { return m_name; }
    std::vector<ComplexInvestment> getInvestments() const { return m_investments; }
};

class TransactionalPortfolioManager {

    std::vector<std::unique_ptr<TransactionalPortfolio>> m_portfolios;

public:
    TransactionalPortfolioManager() = default;
    TransactionalPortfolioManager(const TransactionalPortfolioManager& other) {
        for (const auto& portfolio : other.m_portfolios) {
            m_portfolios.push_back(std::make_unique<TransactionalPortfolio>(*portfolio));
        }
    }
    bool operator==(const TransactionalPortfolioManager& other) const {
        bool retVal = m_portfolios.size() == other.m_portfolios.size();
        if (retVal) {
            retVal = equal(begin(m_portfolios), end(m_portfolios),
                           begin(other.m_portfolios), end(other.m_portfolios),
                           [](const std::unique_ptr<portfolio::TransactionalPortfolio>& lhs,
                              const std::unique_ptr<portfolio::TransactionalPortfolio>& rhs) {
                                return *lhs == *rhs;
                            });
        }
        return retVal;
    }
    bool operator!=(const TransactionalPortfolioManager& other) const {
        return !(*this == other);
    }

    void addPortfolio(TransactionalPortfolio& portfolio);

    bool addPortfolio(const std::string& portfolio_name);

    bool removePortfolio(const std::string& portfolio_name);

    uint16_t getNumPortfolios() const {
        return static_cast<uint16_t>(m_portfolios.size());
    }

    TransactionalPortfolio& getPortfolio(uint16_t portfolio_idx) const {
        return *(m_portfolios[portfolio_idx]);
    }
};

} // namespace portfolio

#endif /* PORTFOLIO_HPP_ */