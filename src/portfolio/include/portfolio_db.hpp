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

class DatabaseComplexInterface {
public:
    virtual bool createTable(const db_manager::TableDetails& tableDetails) = 0;
    virtual bool saveData(const db_manager::TableDetails& tableDetails, const db_manager::values_t& values) = 0;
    virtual bool removeData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId) = 0;
    virtual bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                            const std::string& itemName, const std::string& newValue) = 0;
    virtual bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                            const db_manager::values_t& values) = 0;
    virtual bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                            const std::unordered_map<std::string, std::string>& columnsValues) = 0;
    virtual bool getData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                         std::unordered_map<std::string, std::string>& result) = 0;
    virtual bool listData(const db_manager::TableDetails& tableDetails, db_manager::values_t& result) = 0;
    virtual bool listData(const db_manager::TableDetails& tableDetails,
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
    bool createTable(const db_manager::TableDetails& tableDetails) override;
    bool saveData(const db_manager::TableDetails& tableDetails, const db_manager::values_t& values) override;
    bool removeData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId) override;
    bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                    const std::string& itemName, const std::string& newValue) override;
    bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                    const db_manager::values_t& values) override;
    bool updateData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                    const std::unordered_map<std::string, std::string>& columnsValues) override;
    bool getData(const db_manager::TableDetails& tableDetails, const std::string& uniqueId,
                 std::unordered_map<std::string, std::string>& result) override;
    bool listData(const db_manager::TableDetails& tableDetails, db_manager::values_t& result) override;
    bool listData(const db_manager::TableDetails& tableDetails,
                  std::vector<std::unordered_map<std::string, std::string>>& result) override;
};

class DatabaseComplexInvestments {
    db_manager::TableDetails m_investmentsTable;
    db_manager::TableDetails m_transactionsTable;
    std::shared_ptr<DatabaseComplexInterfaceImplementation> m_dbInterface;
public:
    DatabaseComplexInvestments() = default;
    DatabaseComplexInvestments(const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                               const std::string& dbPath);
    bool createTables();
    bool saveComplexInvestment(const ComplexInvestment& cInvestment);
    bool removeComplexInvestment(const std::string& id);
    bool updateInvestment(const ComplexInvestment& cInvestment);
    bool getInvestment(const std::string& id, ComplexInvestment& cInvestment);
    bool listInvestment(std::vector<ComplexInvestment>& cInvestments);
private:
    db_manager::TableDetails setUpPredefinedInvestmentsTable();
    db_manager::TableDetails setUpPredefinedTransactionsTable();
    bool extractInvestmentFromData(Investment& investment,
                const std::unordered_map<std::string, std::string>& retData);
    bool extractTransactionFromData(Transaction& transaction,
                double_t& currency_rate,
                const std::unordered_map<std::string, std::string>& retData);
    bool extractInvestmentsFromData(std::unordered_map<std::string, Investment>& investments,
                                    const std::vector<std::unordered_map<std::string, std::string>>& retData);
    bool extractTransactionsFromData(std::unordered_map<std::string, std::pair<Transaction, double_t>>& transactions,
                                     const std::vector<std::unordered_map<std::string, std::string>>& retData);
};

} // namespace portfolio

#endif /* PORTFOLIO_DB_HPP_ */