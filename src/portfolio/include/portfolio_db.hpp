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
    virtual bool getInvestment(const std::string& ticker, Investment& investment) = 0;
    virtual ~DatabaseInterface() {}
};

class DatabaseInterfaceImplementation : public DatabaseInterface {
private:
    db_manager::DatabaseORM m_db;
    std::string m_dbPath;
    std::string m_tableName;
    db_manager::columns_t m_columns;
public:
    DatabaseInterfaceImplementation(db_manager::DatabaseORM& db,
                                    const std::string& dbPath,
                                    const std::string& tableName);
    bool saveInvestment(const Investment& investment) override;
    bool updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) override;
    bool updateInvestmentPrice(const std::string& ticker, const double_t price) override;
    bool removeInvestment(const std::string& ticker) override;
    bool getInvestment(const std::string& ticker, Investment& investment) override;
};

DatabaseInterfaceImplementation setUpDb(const std::string& dbDirPath);

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */