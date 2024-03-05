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
    virtual bool updateInvestmentQuantity(const std::string& ticker, const double_t quantity) = 0;
    virtual bool updateInvestmentPurchasePrice(const std::string& ticker, const double_t price) = 0;
    virtual bool updateInvestmentCurrentPrice(const std::string& ticker, const double_t price) = 0;
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
    bool updateInvestmentQuantity(const std::string& ticker, const double_t quantity) override;
    bool updateInvestmentPurchasePrice(const std::string& ticker, const double_t price) override;
    bool updateInvestmentCurrentPrice(const std::string& ticker, const double_t price) override;
    bool removeInvestment(const std::string& ticker) override;
    bool getInvestment(const std::string& ticker, Investment& investment) override;
    bool listInvestments(std::vector<Investment>& investments) override;
};

class DatabaseTransactionalInterface {
public:
    virtual bool createTable(const std::string& tableName) = 0;
    virtual bool saveInvestment(const ComplexInvestment& investment) = 0;
    virtual bool savePrice(const std::string& ticker, const DateTime& date,
                           const std::string& exchange, const double_t price) = 0;
    virtual bool removeInvestment(const std::string& id) = 0;
    virtual bool removePrice(const std::string& ticker) = 0;
    virtual bool updateInvestmentQuantity(const std::string& id, const double_t quantity) = 0;
    virtual bool updatePrice(const std::string& ticker, const double_t price) = 0;
    virtual bool getInvestment(const std::string& id, ComplexInvestment& investment) = 0;
    virtual bool getPrice(const std::string& ticker, double_t& price) = 0;
    virtual bool listInvestments(std::vector<ComplexInvestment>& investments) = 0;
    virtual bool listPrices(std::map<std::string, double_t>& prices) = 0;
};

class DatabaseTransactionalImplementation : public DatabaseTransactionalInterface {
    std::shared_ptr<db_manager::DatabaseStrategy> m_dbStrategy;
    db_manager::DatabaseORM m_dbOrm;
    std::string m_dbPath;
    std::vector<std::string> m_tableNames;
    std::map<uint16_t, db_manager::columns_t> m_mapTableIdsColumns;
    std::map<uint16_t, std::vector<db_manager::columnDefinition_t>> m_mapTableIdsColumnDefinitions;
public:
    DatabaseTransactionalImplementation(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                                        const std::string& dbPath,
                                        const std::vector<std::string>& tableNames);
    bool createTable(const std::string& tableName) override;
    bool saveInvestment(const ComplexInvestment& investment) override;
    bool savePrice(const std::string& ticker, const DateTime& date,
                   const std::string& exchange, const double_t price) override;
    bool removeInvestment(const std::string& id) override;
    bool removePrice(const std::string& ticker) override;
    bool updateInvestmentQuantity(const std::string& id, const double_t quantity) override;
    bool updatePrice(const std::string& ticker, const double_t price) override;
    bool getInvestment(const std::string& id, ComplexInvestment& investment) override;
    bool getPrice(const std::string& ticker, double_t& price) override;
    bool listInvestments(std::vector<ComplexInvestment>& investments) override;
    bool listPrices(std::map<std::string, double_t>& prices) override;

private:
    bool fillTableParameters(const std::string& tableToFill, const db_manager::columns_t& columns,
                             const std::vector<db_manager::columnDefinition_t>& columnDefinitions);
    void defineInvestmentsTable();
    void defineMarketPricesTable();
    void defineTransactionsTable();
};

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */