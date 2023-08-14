#ifndef STRATEGY_DB
#define STRATEGY_DB

#include <string>
#include <vector>
#include <unordered_map>
#include <sqlite3.h>
#include "utils.hpp"

namespace db_manager {

typedef std::vector<std::string> columns_t;
typedef std::vector<std::string> values_t;
typedef std::unordered_map<std::string, std::string> columnDefinitions_t;

struct DbData {
    std::string dbPath;
    std::string dbTable;
    std::string dbColumn;
    std::string dbData;
};

class DatabaseStrategy {
public:

    virtual bool connect(const std::string& dbPath) = 0;
    virtual void disconnect() = 0;
    virtual bool executeQuery(const std::string& query) = 0;
};

class SQLiteStrategy : public DatabaseStrategy {
private:
    sqlite3* m_db;

public:
    bool connect(const std::string& dbPath) override;
    void disconnect() override;
    bool executeQuery(const std::string& query) override;
};

class DatabaseORM {
private:
    DatabaseStrategy* m_strategy;

    std::string convertValues2String(const values_t& values) {
        return convertVectorToString(values);
    }
    std::string convertColumns2String(const columns_t& columns) {
        return convertVectorToString(columns);
    }

public:
    DatabaseORM(DatabaseStrategy* strategy) : m_strategy(strategy) {}

    void createTable(const std::string& tableName,
                     const std::string& columnDefinitions);

    bool save(const std::string& db,
              const std::string& table,
              const columns_t& columns,
              const values_t& values);

    ~DatabaseORM() {
        delete m_strategy;
    }
};

} // namespace db_manager


#endif /* STRATEGY_DB */