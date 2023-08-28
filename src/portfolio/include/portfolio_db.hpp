#ifndef PORTFOLIO_DB_HPP_
#define PORTFOLIO_DB_HPP_

#include <string>
#include "investment.hpp"
#include "strategy_db.hpp"

namespace portfolio {

class DatabaseInterface {
public:
    virtual bool saveInvestment(const Investment& investment) = 0;
    virtual bool updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) = 0;
    virtual bool updateInvestmentPrice(const std::string& ticker, const double_t price) = 0;
    virtual bool removeInvestment(const std::string& ticker) = 0;
    virtual uint32_t getInvestmentQuantity(const std::string& ticker) = 0;
    virtual double_t getInvestmentPrice(const std::string& ticker) = 0;
    virtual ~DatabaseInterface() {}
};

class DatabaseInterfaceImplementation : public DatabaseInterface {
private:
    db_manager::DatabaseORM m_db;
public:
    DatabaseInterfaceImplementation(db_manager::DatabaseORM& db) : m_db(db) {}
    bool saveInvestment(const Investment& investment) override;
    bool updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) override;
    bool updateInvestmentPrice(const std::string& ticker, const double_t price) override;
    bool removeInvestment(const std::string& ticker) override;
    uint32_t getInvestmentQuantity(const std::string& ticker) override;
    double_t getInvestmentPrice(const std::string& ticker) override;
};

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */