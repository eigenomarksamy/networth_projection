#include "portfolio_db.hpp"

portfolio::DatabaseInterfaceImplementation::DatabaseInterfaceImplementation(
                                                const std::shared_ptr<db_manager::DatabaseStrategy>& db_strategy,
                                                const std::string& dbPath,
                                                const std::string& tableName)
  : m_dbStrategy(db_strategy), m_dbPath(dbPath), m_tableName(tableName),
    m_dbOrm(db_manager::DatabaseORM(m_dbStrategy)) {
    m_columns = {"name", "ticker", "purchase_price", "quantity", "current_price"};
}

bool portfolio::DatabaseInterfaceImplementation::createTable() {
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"name", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"purchase_price", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"quantity", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"current_price", "REAL NOT NULL DEFAULT 0.0"});
    return m_dbOrm.createTable(m_dbPath, m_tableName, columnDefinitions);
}

bool portfolio::DatabaseInterfaceImplementation::saveInvestment(const Investment& investment) {
    db_manager::values_t values {"'" + investment.getName() + "'",
                                 "'" + investment.getTicker() + "'",
                                 std::to_string(investment.getPurchasePrice()),
                                 std::to_string(investment.getQuantity()),
                                 std::to_string(investment.getCurrentPrice())};
    if (m_dbOrm.save(m_dbPath, m_tableName, m_columns, values))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentQuantity(const std::string& ticker, const double_t quantity) {
    if (m_dbOrm.update(m_dbPath, m_tableName, "'" + ticker + "'", "ticker", "quantity", std::to_string(quantity)))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentPurchasePrice(const std::string& ticker, const double_t price) {
    if (m_dbOrm.update(m_dbPath, m_tableName, "'" + ticker + "'", "ticker", "purchase_price", std::to_string(price)))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentCurrentPrice(const std::string& ticker, const double_t price) {
    if (m_dbOrm.update(m_dbPath, m_tableName, "'" + ticker + "'", "ticker", "current_price", std::to_string(price)))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::removeInvestment(const std::string& ticker) {
    if (m_dbOrm.remove(m_dbPath, m_tableName, "'" + ticker + "'", "ticker"))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::getInvestment(const std::string& ticker,
                                                               Investment& investment) {
    bool retFlag = true;
    std::string retVal;
    for (const auto& column : m_columns) {
        std::string retVal;
        if ("ticker" != column) {
            retFlag &= m_dbOrm.get(m_dbPath, m_tableName, "ticker", "'" + ticker + "'", column, retVal);
        }
        if ("quantity" == column && retFlag) {
            auto retValI = std::stod(retVal);
            investment.setQuantity(retValI);
        }
        else if ("purchase_price" == column && retFlag) {
            auto retValD = std::stod(retVal);
            investment.setPurchasePrice(retValD);
        }
        else if ("name" == column && retFlag) {
            investment.setName(retVal);
        }
        else if ("current_price" == column && retFlag) {
            auto retValD = std::stod(retVal);
            investment.setCurrentPrice(retValD);
        }
    }
    if (retFlag)
        investment.setTicker(ticker);
    return retFlag;
}

bool portfolio::DatabaseInterfaceImplementation::listInvestments(std::vector<Investment>& investments) {
    bool retFlag;
    std::vector<std::string> output;
    retFlag = m_dbOrm.list(m_dbPath, m_tableName, "ticker", output);
    if (retFlag) {
        for (const auto& out : output) {
            Investment tmpInvestment;
            retFlag = getInvestment(out, tmpInvestment);
            investments.push_back(tmpInvestment);
        }
    }
    return retFlag;
}

bool portfolio::DatabaseComplexInterfaceImplementation::createTable(const TableDetails& tableDetails) {
    return m_dbOrm.createTable(m_dbPath, tableDetails.tableName, tableDetails.columnDefinitions);
}

bool portfolio::DatabaseComplexInterfaceImplementation::saveData(const TableDetails& tableDetails,
                                                                 const db_manager::values_t& values) {
    return m_dbOrm.save(m_dbPath, tableDetails.tableName, tableDetails.columns, values);
}

bool portfolio::DatabaseComplexInterfaceImplementation::removeData(const TableDetails& tableDetails,
                                                                   const std::string& uniqueId) {
    return m_dbOrm.remove(m_dbPath, tableDetails.tableName, uniqueId, tableDetails.primaryKeyName);
}

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const TableDetails& tableDetails,
                                                                   const std::string& uniqueId,
                                                                   const std::string& itemName,
                                                                   const std::string& newValue) {
    return m_dbOrm.update(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName,
                          uniqueId, itemName, newValue);
}

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const TableDetails& tableDetails,
                                                                   const std::string& uniqueId,
                                                                   const db_manager::values_t& values) {
    bool retVal = (tableDetails.columns.size() == values.size());
    for (auto i = 0; i < values.size(); ++i) {
        if (retVal && tableDetails.columns[i] != tableDetails.primaryKeyName) {
            retVal &= m_dbOrm.update(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName,
                                     uniqueId, tableDetails.columns[i], values[i]);
        }
    }
    return retVal;
}

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const TableDetails& tableDetails,
                const std::string& uniqueId,
                const std::unordered_map<std::string, std::string>& columnsValues) {
    bool retVal = (tableDetails.columns.size() == columnsValues.size());
    for (auto it = columnsValues.begin(); it != columnsValues.end(); ++it) {
        if (retVal && it->first != tableDetails.primaryKeyName) {
            retVal &= m_dbOrm.update(m_dbPath, tableDetails.tableName,
                                    tableDetails.primaryKeyName, uniqueId,
                                    it->first, it->second);
        }
    }
    return retVal;
}

bool portfolio::DatabaseComplexInterfaceImplementation::getData(
                                        const TableDetails& tableDetails,
                                        const std::string& uniqueId,
                                        std::unordered_map<std::string, std::string>& result) {
    bool retFlag = true;
    std::string retVal;
    result.clear();
    for (const auto& column : tableDetails.columns) {
        retFlag &= m_dbOrm.get(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName,
                                "'" + uniqueId + "'", column, retVal);
        if (retFlag) {
            result[column] = retVal;
            retVal.clear();
        }
        else {
            break;
        }
    }
    return retFlag;
}

bool portfolio::DatabaseComplexInterfaceImplementation::listData(const TableDetails& tableDetails,
                                                                 db_manager::values_t& result) {
    return m_dbOrm.list(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName, result);
}

bool portfolio::DatabaseComplexInterfaceImplementation::listData(const TableDetails& tableDetails,
                  std::vector<std::unordered_map<std::string, std::string>>& result) {
    return m_dbOrm.list(m_dbPath, tableDetails.tableName, result);
}

portfolio::DatabaseInvestmentsTable::DatabaseInvestmentsTable(
                            const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                            const std::string& dbPath) {
    m_dbInterface = std::make_shared<DatabaseComplexInterfaceImplementation>(dbStrategy, dbPath);
    m_table = setUpPredefinedInvestmentsTable();
}

portfolio::TableDetails portfolio::DatabaseInvestmentsTable::setUpPredefinedInvestmentsTable() {
    TableDetails retTable;
    db_manager::columns_t columns = {"id", "name", "ticker", "purchase_price", "quantity"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"id", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"name", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"purchase_price", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"quantity", "REAL NOT NULL"});
    setUpTable(retTable, std::string("investments"), std::string("id"), columns, columnDefinitions);
    return retTable;
}

bool portfolio::DatabaseInvestmentsTable::createTable() {
    return m_dbInterface->createTable(m_table);
}

bool portfolio::DatabaseInvestmentsTable::saveInvestment(const std::string& id, const Investment& investment) {
    db_manager::values_t values {"'" + id + "'",
                                 "'" + investment.getName() + "'",
                                 "'" + investment.getTicker() + "'",
                                 std::to_string(investment.getPurchasePrice()),
                                 std::to_string(investment.getQuantity())};
    return m_dbInterface->saveData(m_table, values);
}

bool portfolio::DatabaseInvestmentsTable::removeInvestment(const std::string& id) {
    return m_dbInterface->removeData(m_table, id);
}

bool portfolio::DatabaseInvestmentsTable::updateInvestment(const std::string& id, const Investment& investment) {
    db_manager::values_t values {"'" + id + "'",
                                 "'" + investment.getName() + "'",
                                 "'" + investment.getTicker() + "'",
                                 std::to_string(investment.getPurchasePrice()),
                                 std::to_string(investment.getQuantity())};
    return m_dbInterface->updateData(m_table, id, values);
}

bool portfolio::DatabaseInvestmentsTable::getInvestment(const std::string& id, Investment& investment) {
    std::unordered_map<std::string, std::string> retData;
    auto retVal = m_dbInterface->getData(m_table, id, retData);
    if (retVal) {
        for (const auto& column : m_table.columns) {
            if (column == "name") investment.setName(retData[column]);
            else if (column == "ticker") investment.setTicker(retData[column]);
            else if (column == "purchase_price") investment.setPurchasePrice(std::stod(retData[column]));
            else if (column == "quantity") investment.setQuantity(std::stod(retData[column]));
        }
    }
    return retVal;
}

bool portfolio::DatabaseInvestmentsTable::listInvestments(std::unordered_map<std::string, Investment>& investments) {
    std::vector<std::unordered_map<std::string, std::string>> retData;
    auto retVal = m_dbInterface->listData(m_table, retData);
    if (retVal) {
        for (const auto& retMap : retData) {
            bool mapSetForId = false;
            bool mapSetForName = false;
            bool mapSetForTicker = false;
            bool mapSetForPrice = false;
            bool mapSetForQuantity = false;
            std::string id, name, ticker, price, quantity;
            for (const auto& column : m_table.columns) {
                if (column == "id") {
                    id = retMap.at(column);
                    mapSetForId = true;
                }
                else if (column == "name") {
                    name = retMap.at(column);
                    mapSetForName = true;
                }
                else if (column == "ticker") {
                    ticker = retMap.at(column);
                    mapSetForTicker = true;
                }
                else if (column == "purchase_price") {
                    price = retMap.at(column);
                    mapSetForPrice = true;
                }
                else if (column == "quantity") {
                    quantity = retMap.at(column);
                    mapSetForQuantity = true;
                }
            }
            if (mapSetForId && mapSetForName
                && mapSetForPrice && mapSetForQuantity
                && mapSetForTicker) {
                investments[id] = Investment(name, ticker, std::stod(price), std::stod(price), std::stod(quantity));
                retVal &= true;
            }
        }
    }
    return retVal;
}

portfolio::DatabaseTransactionsTable::DatabaseTransactionsTable(
                            const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                            const std::string& dbPath) {
    m_dbInterface = std::make_shared<DatabaseComplexInterfaceImplementation>(dbStrategy, dbPath);
    m_table = setUpPredefinedTransactionsTable();
}

portfolio::TableDetails portfolio::DatabaseTransactionsTable::setUpPredefinedTransactionsTable() {
    TableDetails retTable;
    db_manager::columns_t columns = {"id", "date", "fees", "currency",
                                     "currency_conversion_fees",
                                     "currency_conversion_rate"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"id", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"date", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"fees", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"currency", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"currency_conversion_fees", "REAL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"currency_conversion_rate", "REAL"});
    setUpTable(retTable, std::string("transactions"), std::string("id"), columns, columnDefinitions);
    return retTable;
}

bool portfolio::DatabaseTransactionsTable::createTable() {
    return m_dbInterface->createTable(m_table);
}

bool portfolio::DatabaseTransactionsTable::saveTransaction(const std::string& id, const Transaction& transaction, const double_t currency_rate) {
    db_manager::values_t values {"'" + id + "'"
                                 "'" + transaction.m_datetime.getDateTimeString() + "'",
                                 std::to_string(transaction.m_fees),
                                 "'" + transaction.currency_to_str() + "'",
                                 std::to_string(transaction.m_conversion_fees),
                                 std::to_string(currency_rate)};
    return m_dbInterface->saveData(m_table, values);
}

bool portfolio::DatabaseTransactionsTable::removeInvestment(const std::string& id) {
    return m_dbInterface->removeData(m_table, id);
}

bool portfolio::DatabaseTransactionsTable::updateTransaction(const std::string& id, const Transaction& transaction, const double_t currency_rate) {
    db_manager::values_t values {"'" + id + "'"
                                 "'" + transaction.m_datetime.getDateTimeString() + "'",
                                 std::to_string(transaction.m_fees),
                                 "'" + transaction.currency_to_str() + "'",
                                 std::to_string(transaction.m_conversion_fees),
                                 std::to_string(currency_rate)};
    return m_dbInterface->updateData(m_table, id, values);
}

bool portfolio::DatabaseTransactionsTable::getTransaction(const std::string& id, Transaction& transaction, double_t& currency_rate) {
    std::unordered_map<std::string, std::string> retData;
    auto retVal = m_dbInterface->getData(m_table, id, retData);
    if (retVal) {
        for (const auto& column : m_table.columns) {
            if (column == "date") transaction.m_datetime.setDateTimeFromString(retData[column]);
            else if (column == "fees") transaction.m_fees = std::stod(retData[column]);
            else if (column == "currency") transaction.str_to_currency(retData[column]);
            else if (column == "currency_conversion_fees") transaction.m_conversion_fees = std::stod(retData[column]);
            else if (column == "currency_conversion_rate") currency_rate = std::stod(retData[column]);
        }
    }
    return retVal;
}

bool portfolio::DatabaseTransactionsTable::listTransactions(std::unordered_map<std::string, std::pair<Transaction, double_t>>& transactions) {
    std::vector<std::unordered_map<std::string, std::string>> retData;
    bool retVal = m_dbInterface->listData(m_table, retData);
    if (retVal) {
        for (const auto& retMap : retData) {
            bool mapSetForId = false;
            bool mapSetForDate = false;
            bool mapSetForFees = false;
            bool mapSetForCurrency = false;
            bool mapSetForCCF = false;
            bool mapSetForCCR = false;
            std::string id, date, fees, currency, ccf, ccr;
            for (const auto& column : m_table.columns) {
                if (column == "id") {
                    id = retMap.at(column);
                    mapSetForId = true;
                }
                else if (column == "date") {
                    date = retMap.at(column);
                    mapSetForDate = true;
                }
                else if (column == "fees") {
                    fees = retMap.at(column);
                    mapSetForFees = true;
                }
                else if (column == "currency") {
                    currency = retMap.at(column);
                    mapSetForCurrency = true;
                }
                else if (column == "currency_conversion_fees") {
                    ccf = retMap.at(column);
                    mapSetForCCF = true;
                }
                else if (column == "currency_conversion_rate") {
                    ccr = retMap.at(column);
                    mapSetForCCR = true;
                }
            }
            if (mapSetForId && mapSetForDate
                && mapSetForFees && mapSetForCurrency
                && mapSetForCCF && mapSetForCCR) {
                Transaction transaction;
                transaction.m_conversion_fees = std::stod(ccf);
                transaction.m_fees = std::stod(fees);
                transaction.str_to_currency(currency);
                transaction.m_datetime.setDateTimeFromString(date);
                transactions.at(id) = std::make_pair(transaction, std::stod(ccr));
                retVal &= true;
            }
        }
    }
    return retVal;
}

portfolio::DatabaseMarketTable::DatabaseMarketTable(
                    const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
                    const std::string& dbPath) {
    m_dbInterface = std::make_shared<DatabaseComplexInterfaceImplementation>(dbStrategy, dbPath);
    m_table = setUpPredefinedMarketTable();
}

portfolio::TableDetails portfolio::DatabaseMarketTable::setUpPredefinedMarketTable() {
    TableDetails retTable;
    db_manager::columns_t columns = {"ticker", "date", "exchange", "price"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"date", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"exchange", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"price", "REAL NOT NULL"});
    setUpTable(retTable, std::string("market"), std::string("ticker"), columns, columnDefinitions);
    return retTable;
}

bool portfolio::DatabaseMarketTable::createTable() {
    return m_dbInterface->createTable(m_table);
}

bool portfolio::DatabaseMarketTable::saveMarketEntry(const std::string& ticker, const DateTime& datetime, const std::string& exchange, const double_t price) {
    db_manager::values_t values {"'" + ticker + "'"
                                 "'" + datetime.getDateTimeString() + "'",
                                 "'" + exchange + "'",
                                 std::to_string(price)};
    return m_dbInterface->saveData(m_table, values);
}

bool portfolio::DatabaseMarketTable::removeMarketEntry(const std::string& ticker) {
    return m_dbInterface->removeData(m_table, ticker);
}

bool portfolio::DatabaseMarketTable::updateMarketEntry(const std::string& ticker, const DateTime& datetime, const std::string& exchange, const double_t price) {
    db_manager::values_t values {"'" + ticker + "'"
                                 "'" + datetime.getDateTimeString() + "'",
                                 "'" + exchange + "'",
                                 std::to_string(price)};
    return m_dbInterface->updateData(m_table, ticker, values);
}

bool portfolio::DatabaseMarketTable::getMarketEntry(const std::string& ticker, DateTime& datetime, std::string& exchange, double_t& price) {
    std::unordered_map<std::string, std::string> retData;
    auto retVal = m_dbInterface->getData(m_table, ticker, retData);
    if (retVal) {
        for (const auto& column : m_table.columns) {
            if (column == "ticker") continue;
            else if (column == "date") datetime.setDateTimeFromString(retData[column]);
            else if (column == "exchange") exchange = retData[column];
            else if (column == "price") price = std::stod(retData[column]);
        }
    }
    return retVal;
}

bool portfolio::DatabaseMarketTable::listMarketEntries(std::unordered_map<std::string, std::pair<DateTime, std::pair<std::string, double_t>>>& entries) {
    std::vector<std::unordered_map<std::string, std::string>> retData;
    bool retVal = m_dbInterface->listData(m_table, retData);
    if (retVal) {
        for (const auto& retMap : retData) {
            bool mapSetForTicker = false;
            bool mapSetForDate = false;
            bool mapSetForExchange = false;
            bool mapSetForPrice = false;
            std::string ticker, date, exchange, price;
            for (const auto& column : m_table.columns) {
                if (column == "ticker") {
                    ticker = retMap.at(column);
                    mapSetForTicker = true;
                }
                else if (column == "date") {
                    date = retMap.at(column);
                    mapSetForDate = true;
                }
                else if (column == "exchange") {
                    exchange = retMap.at(column);
                    mapSetForExchange = true;
                }
                else if (column == "price") {
                    price = retMap.at(column);
                    mapSetForPrice = true;
                }
            }
            if (mapSetForTicker && mapSetForDate
                && mapSetForExchange && mapSetForPrice) {
                DateTime date_time = DateTime(0, 0, 0, 0, 0, 0);
                date_time.setDateTimeFromString(date);
                entries.at(ticker) = std::make_pair(date_time, std::make_pair(exchange, std::stod(price)));
                retVal &= true;
            }
        }
    }
    return retVal;
}

void portfolio::setUpTable(TableDetails& tableDetails, const std::string& tableName,
                           const std::string& primaryKeyName, const db_manager::columns_t& columns,
                           const std::vector<db_manager::columnDefinition_t>& columnDefinitions) {
    tableDetails.tableName = tableName;
    tableDetails.primaryKeyName = primaryKeyName;
    tableDetails.columns = columns;
    tableDetails.columnDefinitions = columnDefinitions;
}
