#include "strategy_db.hpp"
#include <iostream>

bool db_manager::SQLiteStrategy::connect(const std::string& dbPath) {
    auto result = sqlite3_open(dbPath.c_str(), &m_db);
    return result == SQLITE_OK;
}

void db_manager::SQLiteStrategy::disconnect() {
    sqlite3_close(m_db);
}

bool db_manager::SQLiteStrategy::executeQuery(const std::string& query) {
    char* errorMessage;
    auto result = sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "SQLite error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

bool db_manager::DatabaseORM::save(const std::string& db,
                                   const std::string& table,
                                   const columns_t& columns,
                                   const values_t& values) {
    auto column_str = convertColumns2String(columns);
    auto value_str = convertValues2String(values);
    std::string insertDataQuery = "INSERT INTO " + table + " (" + column_str + ") "
                                  "VALUES (" + value_str + ")";
    bool ret = m_strategy->connect(db);
    if (!ret) {
        std::cerr << "Error connecting\n";
        return false;
    }
    ret = m_strategy->executeQuery(insertDataQuery);
    if (!ret) {
        std::cerr << "Error executing\n";
        return false;
    }
    m_strategy->disconnect();
    return true;
}