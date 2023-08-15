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

std::string db_manager::DatabaseORM::convertColumnDefinitions2String(const std::vector<columnDefinition_t>& columnDefinitions) {
    std::string str = "";
    uint16_t i = 0;
    for (i = 0; i < columnDefinitions.size() - 1; ++i) {
        str += columnDefinitions[i].column + " " + columnDefinitions[i].definition + ",";
    }
    str += columnDefinitions[i].column + " " + columnDefinitions[i].definition;
    return str;
}

bool db_manager::DatabaseORM::createTable(const std::string& db,
                                          const std::string& table,
                                          const std::vector<columnDefinition_t>& columnDefinitions) const {
    auto columnDefinitions_str = convertColumnDefinitions2String(columnDefinitions);
    std::string query = "CREATE TABLE IF NOT EXISTS " + table + " ("
                        + columnDefinitions_str + ")";
    bool ret = m_strategy->connect(db);
    if (!ret) {
        std::cerr << "Error connecting (Create)\n";
        return false;
    }
    ret = m_strategy->executeQuery(query);
    if (!ret) {
        std::cerr << "Error executing (Create)\n";
        return false;
    }
    m_strategy->disconnect();
    return true;
}

bool db_manager::DatabaseORM::save(const std::string& db,
                                   const std::string& table,
                                   const columns_t& columns,
                                   const values_t& values) const {
    auto column_str = convertColumns2String(columns);
    auto value_str = convertValues2String(values);
    std::string query = "INSERT INTO " + table + " (" + column_str + ") "
                        "VALUES (" + value_str + ")";
    bool ret = m_strategy->connect(db);
    if (!ret) {
        std::cerr << "Error connecting (Save)\n";
        return false;
    }
    ret = m_strategy->executeQuery(query);
    if (!ret) {
        std::cerr << "Error executing (Save)\n";
        return false;
    }
    m_strategy->disconnect();
    return true;
}

bool db_manager::DatabaseORM::update(const std::string& db,
                                     const std::string& table,
                                     const std::string& keyValue,
                                     const std::string& keyName,
                                     const std::string& column,
                                     const std::string& value) const {
    std::string query = "UPDATE " + table + " SET " + column + " = " + value
                        + " WHERE " + keyName + " = " + keyValue;
    bool ret = m_strategy->connect(db);
    if (!ret) {
        std::cerr << "Error connecting (Update)\n";
        return false;
    }
    ret = m_strategy->executeQuery(query);
    if (!ret) {
        std::cerr << "Error executing (Update)\n";
        return false;
    }
    m_strategy->disconnect();
    return true;
}

bool db_manager::DatabaseORM::remove(const std::string& db,
                                     const std::string& table,
                                     const std::string& keyValue,
                                     const std::string& keyName) const {
    std::string query = "DELETE FROM " + table + " WHERE " + keyName + " = " + keyValue;
    bool ret = m_strategy->connect(db);
    if (!ret) {
        std::cerr << "Error connecting (Remove)\n";
        return false;
    }
    ret = m_strategy->executeQuery(query);
    if (!ret) {
        std::cerr << "Error executing (Remove)\n";
        return false;
    }
    m_strategy->disconnect();
    return true;
}