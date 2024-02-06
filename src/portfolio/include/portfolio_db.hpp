#ifndef PORTFOLIO_DB_HPP_
#define PORTFOLIO_DB_HPP_

#include <string>
#include <memory>
#include "investment.hpp"
#include "strategy_db.hpp"

namespace portfolio {

class DatabaseInterface {
public:
    virtual bool createTable() = 0;
    virtual bool saveInvestment(const Investment& investment) = 0;
    virtual bool updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) = 0;
    virtual bool updateInvestmentPrice(const std::string& ticker, const double_t price) = 0;
    virtual bool removeInvestment(const std::string& ticker) = 0;
    virtual bool getInvestment(const std::string& ticker, Investment& investment) = 0;
    virtual bool listInvestments(std::vector<Investment>& investments) = 0;
};

class DatabaseInterfaceImplementation : public DatabaseInterface {
private:
    std::shared_ptr<db_manager::DatabaseStrategy> m_dbStrategy;
    db_manager::DatabaseORM m_dbOrm;
    std::string m_dbPath;
    std::string m_tableName;
    db_manager::columns_t m_columns;
public:
    DatabaseInterfaceImplementation(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy ,
                                    const std::string& dbPath,
                                    const std::string& tableName);
    bool createTable() override;
    bool saveInvestment(const Investment& investment) override;
    bool updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) override;
    bool updateInvestmentPrice(const std::string& ticker, const double_t price) override;
    bool removeInvestment(const std::string& ticker) override;
    bool getInvestment(const std::string& ticker, Investment& investment) override;
    bool listInvestments(std::vector<Investment>& investments) override;
};

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */