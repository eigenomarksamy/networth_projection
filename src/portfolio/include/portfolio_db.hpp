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

struct TableDetails {
    std::string tableName;
    std::string primaryKeyName;
    db_manager::columns_t columns;
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
};

class DatabaseComplexInterface {
public:
    virtual bool createTable(const TableDetails& tableDetails) = 0;
    virtual bool saveData(const TableDetails& tableDetails, const db_manager::values_t& values) = 0;
    virtual bool removeData(const TableDetails& tableDetails, const std::string& uniqueId) = 0;
    virtual bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                            const std::string& itemName, const std::string& newValue) = 0;
    virtual bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                            const db_manager::values_t& values) = 0;
    virtual bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                            const std::unordered_map<std::string, std::string>& columnsValues) = 0;
    virtual bool getData(const TableDetails& tableDetails, const std::string& uniqueId,
                         std::unordered_map<std::string, std::string>& result) = 0;
    virtual bool listData(const TableDetails& tableDetails, db_manager::values_t& result) = 0;
    virtual bool listData(const TableDetails& tableDetails,
                          std::vector<std::unordered_map<std::string, std::string>>& result) = 0;
};

class DatabaseComplexInterfaceImplementation : public DatabaseComplexInterface {
    std::shared_ptr<db_manager::DatabaseStrategy> m_dbStrategy;
    db_manager::DatabaseORM m_dbOrm;
    std::string m_dbPath;
public:
    DatabaseComplexInterfaceImplementation(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                                           const std::string& dbPath)
      : m_dbStrategy(dbStrategy), m_dbPath(dbPath), m_dbOrm(db_manager::DatabaseORM(m_dbStrategy)) {}
    bool createTable(const TableDetails& tableDetails) override;
    bool saveData(const TableDetails& tableDetails, const db_manager::values_t& values) override;
    bool removeData(const TableDetails& tableDetails, const std::string& uniqueId) override;
    bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                    const std::string& itemName, const std::string& newValue) override;
    bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                    const db_manager::values_t& values) override;
    bool updateData(const TableDetails& tableDetails, const std::string& uniqueId,
                    const std::unordered_map<std::string, std::string>& columnsValues) override;
    bool getData(const TableDetails& tableDetails, const std::string& uniqueId,
                 std::unordered_map<std::string, std::string>& result) override;
    bool listData(const TableDetails& tableDetails, db_manager::values_t& result) override;
    bool listData(const TableDetails& tableDetails,
                  std::vector<std::unordered_map<std::string, std::string>>& result) override;
};


void setUpTable(TableDetails& tableDetails, const std::string& tableName,
                const std::string& primaryKeyName, const db_manager::columns_t& columns,
                const std::vector<db_manager::columnDefinition_t>& columnDefinitions);

class DatabaseInvestmentsTable {
    TableDetails m_table;
    std::shared_ptr<DatabaseComplexInterfaceImplementation> m_dbInterface;
public:
    DatabaseInvestmentsTable() = default;
    DatabaseInvestmentsTable(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                             const std::string& dbPath);
    bool createTable();
    bool saveInvestment(const std::string& id, const Investment& investment);
    bool removeInvestment(const std::string& id);
    bool updateInvestment(const std::string& id, const Investment& investment);
    bool getInvestment(const std::string& id, Investment& investment);
    bool listInvestments(std::unordered_map<std::string, Investment>& investments);
private:
    TableDetails setUpPredefinedInvestmentsTable();
};

class DatabaseTransactionsTable {
    TableDetails m_table;
    std::shared_ptr<DatabaseComplexInterfaceImplementation> m_dbInterface;
public:
    DatabaseTransactionsTable() = default;
    DatabaseTransactionsTable(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                              const std::string& dbPath);
    bool createTable();
    bool saveTransaction(const std::string& id, const Transaction& transaction, const double_t currency_rate);
    bool removeInvestment(const std::string& id);
    bool updateTransaction(const std::string& id, const Transaction& transaction, const double_t currency_rate);
    bool getTransaction(const std::string& id, Transaction& transaction, double_t& currency_rate);
    bool listTransactions(std::unordered_map<std::string, std::pair<Transaction, double_t>>& transactions);
private:
    TableDetails setUpPredefinedTransactionsTable();
};

class DatabaseMarketTable {
    TableDetails m_table;
    std::shared_ptr<DatabaseComplexInterfaceImplementation> m_dbInterface;
public:
    DatabaseMarketTable() = default;
    DatabaseMarketTable(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                        const std::string& dbPath);
    bool createTable();
    bool saveMarketEntry(const std::string& ticker, const DateTime& datetime, const std::string& exchange, const double_t price);
    bool removeMarketEntry(const std::string& ticker);
    bool updateMarketEntry(const std::string& ticker, const DateTime& datetime, const std::string& exchange, const double_t price);
    bool getMarketEntry(const std::string& ticker, DateTime& datetime, std::string& exchange, double_t& price);
    bool listMarketEntries(std::unordered_map<std::string, std::pair<DateTime, std::pair<std::string, double_t>>>& entries);
private:
    TableDetails setUpPredefinedMarketTable();
};

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */