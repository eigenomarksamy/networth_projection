#include "strategy_db.hpp"
#include <iostream>

void db_manager::DatabaseStrategy::handleError(const Operation& operation) {
    m_error = operation;
}

db_manager::DatabaseStrategy::Operation db_manager::DatabaseStrategy::getError() {
    return m_error;
}

db_manager::SQLiteStrategy::SQLiteStrategy(const char* dbPath) {
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        handleError(Operation::OPEN);
    }
}

void db_manager::SQLiteStrategy::disconnect() {
    if (sqlite3_close(db) != SQLITE_OK) {
        handleError(Operation::DISCONNECT);
    }
}

void db_manager::SQLiteStrategy::executeQuery(const std::string& query) {
    char* errMsg;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite error: " << errMsg << std::endl;
        handleError(Operation::EXECUTE);
        sqlite3_free(errMsg);
    }
}

void db_manager::DatabaseORM::createTable(const std::string& tableName,
                                          const std::string& columnDefinitions) {
    std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " ("
                        "id INTEGER PRIMARY KEY," + columnDefinitions + ")";
    strategy->connect();
    strategy->executeQuery(query);
    strategy->disconnect();
}

void db_manager::DatabaseORM::save(const std::string& tableName,
                                   const std::string& columnName,
                                   const std::string& data) {
    std::string query = "INSERT INTO " + tableName + " (" + columnName
                        + ") VALUES ('" + data + "')";
    strategy->connect();
    strategy->executeQuery(query);
    strategy->disconnect();
}

bool db_manager::executeDbSave(const DbData& toSave) {
    bool retVal = false;
    const char* db_path = toSave.dbPath.c_str();
    auto strategy = new SQLiteStrategy(db_path);
    DatabaseORM databaseORM(strategy);
    databaseORM.save(toSave.dbTable, toSave.dbColumn, toSave.dbData);
    if (strategy->getError() == DatabaseStrategy::Operation::NONE) {
        retVal = true;
    }
    else {
        auto err = strategy->getError();
        if (err == DatabaseStrategy::Operation::CONNECT) {
            std::cout << "Connect\n";
        }
        else if (err == DatabaseStrategy::Operation::DISCONNECT) {
            std::cout << "Disconnect\n";
        }
        else if (err == DatabaseStrategy::Operation::EXECUTE) {
            std::cout << "Execute\n";
        }
        else if (err == DatabaseStrategy::Operation::OPEN) {
            std::cout << "Open\n";
        }
        else {
            std::cout << "WTF!\n";
        }
    }
    delete strategy;
    return retVal;
}