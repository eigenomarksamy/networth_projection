#ifndef STRATEGY_DB
#define STRATEGY_DB

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sqlite3.h>
#include "utils.hpp"

namespace db_manager {

typedef std::vector<std::string> columns_t;
typedef std::vector<std::string> values_t;
typedef struct {
    std::string column;
    std::string definition;
} columnDefinition_t;

struct DbData {
    std::string dbPath;
    std::string dbTable;
    std::string dbColumn;
    std::string dbData;
};

struct TableDetails {
    std::string tableName;
    std::string primaryKeyName;
    db_manager::columns_t columns;
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
};

class DatabaseStrategy {
public:

    typedef std::vector<std::unordered_map<std::string, std::string>> QueryResult_t;

    virtual bool connect(const std::string& dbPath) = 0;
    virtual void disconnect() = 0;
    virtual bool executeQuery(const std::string& query) = 0;
    virtual QueryResult_t getResults() const = 0;
    virtual void clearResults() = 0;
};

class SQLiteStrategy : public DatabaseStrategy {
private:
    sqlite3* m_db;
    QueryResult_t m_queryResults;

public:
    bool connect(const std::string& dbPath) override;
    void disconnect() override;
    bool executeQuery(const std::string& query) override;
    QueryResult_t getResults() const override {
        return m_queryResults;
    }
    void clearResults() override {
        m_queryResults.clear();
    }
    ~SQLiteStrategy() {
        if (m_db) {
            sqlite3_close(m_db);
        }
    }

private:
    static int callback(void* data, int argc, char** argv, char** azColName) {
        SQLiteStrategy* instance = static_cast<SQLiteStrategy*>(data);
        std::unordered_map<std::string, std::string> row;
        for (int i = 0; i < argc; ++i) {
            std::string colName = azColName[i];
            std::string colValue = (argv[i]) ? argv[i] : "NULL";
            row[colName] = colValue;
        }
        instance->m_queryResults.push_back(row);
        return 0;
    }
};

class DatabaseORM {
private:
    std::shared_ptr<DatabaseStrategy> m_strategy;

    static std::string convertValues2String(const values_t& values) {
        return convertVectorToString(values);
    }
    static std::string convertColumns2String(const columns_t& columns) {
        return convertVectorToString(columns);
    }
    static std::string convertColumnDefinitions2String(const std::vector<columnDefinition_t>& columnDefinitions);
    static uint16_t getColumnIdxFromName(const columns_t& columns,
                                         const std::string& columnName);

    static std::string getCreateTableQuery(const std::string& table,
                               const std::string& columnDefinitions) {
        return "CREATE TABLE IF NOT EXISTS " + table + " ("
                            + columnDefinitions + ")";
    }
    static std::string getSaveQuery(const std::string& table,
                                    const std::string& columns,
                                    const std::string& values) {
        return "INSERT INTO " + table + " (" + columns + ") "
                        "VALUES (" + values + ")";
    }
    static std::string getUpdateQuery(const std::string& table,
                                      const std::string& keyName,
                                      const std::string& keyValue,
                                      const std::string& column,
                                      const std::string& value) {
        return "UPDATE " + table + " SET " + column + " = " + value
                        + " WHERE " + keyName + " = " + keyValue;
    }
    static std::string getRemoveQuery(const std::string& table,
                                      const std::string& keyName,
                                      const std::string& keyValue) {
        return "DELETE FROM " + table + " WHERE "
                        + keyName + " = " + keyValue;
    }
    static std::string getGetQuery(const std::string& table,
                                   const std::string& keyName,
                                   const std::string& keyValue,
                                   const std::string& outputName) {
        return "SELECT " + outputName + " FROM " + table
                        + " WHERE " + keyName + " = " + keyValue;
    }
    static std::string getListQuery(const std::string& table,
                                    const std::string& keyName) {
        return "SELECT " + keyName + " FROM " + table;
    }
    static std::string getListQuery(const std::string& table) {
        return "SELECT * FROM " + table;
    }

public:
    DatabaseORM(std::shared_ptr<DatabaseStrategy> strategy) : m_strategy(strategy) {}

    bool operate(const std::string& db,
                 const std::string& query,
                 DatabaseStrategy::QueryResult_t* result) const;

    bool extractResults(const DatabaseStrategy::QueryResult_t& qResults, 
                        const std::string& outputName,
                        std::string& outputValue) const;

    bool extractResults(const DatabaseStrategy::QueryResult_t& qResults,
                        const std::string& outputName,
                        std::vector<std::string>& outputValues) const;

    bool extractResults(const DatabaseStrategy::QueryResult_t& qResults,
                        std::vector<std::unordered_map<std::string, std::string>>& outputValues) const;

    bool createTable(const std::string& db,
                     const std::string& table,
                     const std::vector<columnDefinition_t>& columnDefinitions) const;

    bool save(const std::string& db,
              const std::string& table,
              const columns_t& columns,
              const values_t& values) const;

    bool update(const std::string& db,
                const std::string& table,
                const std::string& keyName,
                const std::string& keyValue,
                const std::string& column,
                const std::string& value) const;

    bool remove(const std::string& db,
                const std::string& table,
                const std::string& keyName,
                const std::string& keyValue) const;

    bool get(const std::string& db,
             const std::string& table,
             const std::string& keyName,
             const std::string& keyValue,
             const std::string& outputName,
             std::string& outputValue) const;

    bool list(const std::string& db,
              const std::string& table,
              const std::string& keyName,
              std::vector<std::string>& outputList) const;

    bool list(const std::string& db,
              const std::string& table,
              std::vector<std::unordered_map<std::string, std::string>>& outputMap) const;

    ~DatabaseORM() { }
};

} // namespace db_manager


#endif /* STRATEGY_DB */