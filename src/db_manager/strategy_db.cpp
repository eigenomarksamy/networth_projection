#include "strategy_db.hpp"
#include <iostream>

bool db_manager::SQLiteStrategy::connect(const std::string& dbPath) {
    auto result = sqlite3_initialize();
    result |= sqlite3_open(dbPath.c_str(), &m_db);
    return result == SQLITE_OK;
}

void db_manager::SQLiteStrategy::disconnect() {
    sqlite3_close(m_db);
    sqlite3_shutdown();
}

bool db_manager::SQLiteStrategy::executeQuery(const std::string& query) {
    char* errorMessage;
    auto result = sqlite3_exec(m_db, query.c_str(), callback, this, &errorMessage);
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

uint16_t db_manager::DatabaseORM::getColumnIdxFromName(const columns_t& columns,
                                                      const std::string& columnName) {
    uint16_t idx = 0;
    for (const auto& col : columns) {
        if (col == columnName) {
            return idx;
        }
        ++idx;
    }
    return -1;
}

bool db_manager::DatabaseORM::operate(const std::string& db,
                                      const std::string& query,
                                      DatabaseStrategy::QueryResult_t* result) const {
    bool ret = m_strategy.get()->connect(db);
    if (!ret) {
        std::cerr << "Error connecting\n";
        m_strategy->disconnect();
        return false;
    }
    ret = m_strategy->executeQuery(query);
    if (!ret) {
        std::cerr << "Error executing\n";
        m_strategy->disconnect();
        return false;
    }
    if (result != nullptr) {
        *result = m_strategy->getResults();
    }
    m_strategy->disconnect();
    return true;
}

bool db_manager::DatabaseORM::extractResults(const DatabaseStrategy::QueryResult_t& qResults,
                                             const std::string& outputName,
                                             std::string& outputValue) const {
    if (qResults.empty()) {
        return false;
    }
    else {
        auto rowMap = qResults.back();
        auto it = rowMap.find(outputName);
        if (it == rowMap.end()) {
            return false;
        }
        else {
            outputValue = it->second;
        }
    }
    return true;
}

bool db_manager::DatabaseORM::extractResults(const DatabaseStrategy::QueryResult_t& qResults,
                                             const std::string& outputName,
                                             std::vector<std::string>& outputValues) const {
    if (qResults.empty()) {
        return false;
    }
    else {
        for (const auto& qR : qResults) {
            auto it = qR.find(outputName);
            if (it != qR.end()) {
                outputValues.push_back(it->second);
            }
        }
    }
    return true;
}

bool db_manager::DatabaseORM::extractResults(const DatabaseStrategy::QueryResult_t& qResults,
                        std::vector<std::unordered_map<std::string, std::string>>& outputValues) const {
    if (qResults.empty()) {
        return false;
    }
    else {
        for (const auto& qR : qResults) {
            outputValues.push_back(qR);
        }
    }
    return true;
}

bool db_manager::DatabaseORM::createTable(const std::string& db,
                                          const std::string& table,
                                          const std::vector<columnDefinition_t>& columnDefinitions) const {
    auto columnDefinitions_str = convertColumnDefinitions2String(columnDefinitions);
    std::string query = getCreateTableQuery(table, columnDefinitions_str);
    auto ret = operate(db, query, nullptr);
    if (!ret) {
        std::cerr << "Error: Create Table"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    return ret;
}

bool db_manager::DatabaseORM::save(const std::string& db,
                                   const std::string& table,
                                   const columns_t& columns,
                                   const values_t& values) const {
    auto column_str = convertColumns2String(columns);
    auto value_str = convertValues2String(values);
    std::string query = getSaveQuery(table, column_str, value_str);
    auto ret = operate(db, query, nullptr);
    if (!ret) {
        std::cerr << "Error: Save"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    return ret;
}

bool db_manager::DatabaseORM::update(const std::string& db,
                                     const std::string& table,
                                     const std::string& keyName,
                                     const std::string& keyValue,
                                     const std::string& column,
                                     const std::string& value) const {
    std::string query = getUpdateQuery(table, keyName, keyValue, column, value);
    auto ret = operate(db, query, nullptr);
    if (!ret) {
        std::cerr << "Error: Update"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    return ret;
}

bool db_manager::DatabaseORM::remove(const std::string& db,
                                     const std::string& table,
                                     const std::string& keyName,
                                     const std::string& keyValue) const {
    std::string query = getRemoveQuery(table, keyName, keyValue);
    auto ret = operate(db, query, nullptr);
    if (!ret) {
        std::cerr << "Error: Remove"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    return ret;
}

bool db_manager::DatabaseORM::get(const std::string& db,
                                  const std::string& table,
                                  const std::string& keyName,
                                  const std::string& keyValue,
                                  const std::string& outputName,
                                  std::string& outputValue) const {
    std::string query = getGetQuery(table, keyName, keyValue, outputName);
    DatabaseStrategy::QueryResult_t qResult;
    auto ret = operate(db, query, &qResult);
    if (!ret) {
        std::cerr << "Error: Get"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    else {
        ret = extractResults(qResult, outputName, outputValue);
        if (!ret) {
            std::cerr << "For output " << outputName << ", no results found!\n";
        }
    }
    return ret;
}

bool db_manager::DatabaseORM::list(const std::string& db,
                                   const std::string& table,
                                   const std::string& keyName,
                                   std::vector<std::string>& outputList) const {
    std::string query = getListQuery(table, keyName);
    DatabaseStrategy::QueryResult_t qResult;
    auto ret = operate(db, query, &qResult);
    if (!ret) {
        std::cerr << "Error: List"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    else {
        ret = extractResults(qResult, keyName, outputList);
        if (!ret) {
            std::cerr << "For key " << keyName << ", no results found!\n";
        }
    }
    return ret;
}

bool db_manager::DatabaseORM::list(const std::string& db,
                                   const std::string& table,
                                   std::vector<std::unordered_map<std::string, std::string>>& outputMap) const {
    std::string query(table);
    DatabaseStrategy::QueryResult_t qResult;
    auto ret = operate(db, query, &qResult);
    if (!ret) {
        std::cerr << "Error: List"
                  << "\n\tDB: " << db
                  << "\n\tTable: " << table
                  << "\n\tQuery: " << query
                  << std::endl;
    }
    else {
        ret = extractResults(qResult, outputMap);
        if (!ret) {
            std::cerr << "For list, no results found!\n";
        }
    }
    return ret;
}