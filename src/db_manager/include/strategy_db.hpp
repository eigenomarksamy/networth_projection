#ifndef STRATEGY_DB
#define STRATEGY_DB

#include <string>
#include <vector>
#include <unordered_map>
#include <sqlite3.h>

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

public:
    DatabaseORM(DatabaseStrategy* strategy) : m_strategy(strategy) {}

    void createTable(const std::string& tableName,
                     const std::string& columnDefinitions);

    bool save(const std::string& db,
              const std::string& table,
              const std::vector<std::string>& columns,
              const std::vector<std::string>& values);

    ~DatabaseORM() {
        delete m_strategy;
    }
};

} // namespace db_manager


#endif /* STRATEGY_DB */