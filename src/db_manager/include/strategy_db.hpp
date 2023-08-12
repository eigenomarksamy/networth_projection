#ifndef STRATEGY_DB
#define STRATEGY_DB

#include <string>
#include <sqlite3.h>

namespace db_manager {

struct DbData {
    std::string dbPath;
    std::string dbTable;
    std::string dbColumn;
    std::string dbData;
};

class DatabaseStrategy {
public:
    enum class Operation { CONNECT, DISCONNECT, EXECUTE, OPEN, NONE };

    void handleError(const Operation& operation);
    Operation getError();

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void executeQuery(const std::string& query) = 0;
    virtual ~DatabaseStrategy() {}

private:
    Operation m_error = Operation::NONE;
};

class SQLiteStrategy : public DatabaseStrategy {
private:
    sqlite3* db;

public:
    SQLiteStrategy(const char* dbPath);
    void connect() override {}
    void disconnect() override;
    void executeQuery(const std::string& query) override;
    ~SQLiteStrategy() {
        sqlite3_close(db);
    }
};

class DatabaseORM {
private:
    DatabaseStrategy* strategy;

public:
    DatabaseORM(DatabaseStrategy* strategy) : strategy(strategy) {}

    void save(const std::string& tableName,
              const std::string& columnName,
              const std::string& data);
};

bool executeDbSave(const DbData& toSave);

} // namespace db_manager


#endif /* STRATEGY_DB */