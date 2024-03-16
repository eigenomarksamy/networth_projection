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

bool portfolio::DatabaseComplexInterfaceImplementation::createTable(const db_manager::TableDetails& tableDetails) {
    return m_dbOrm.createTable(m_dbPath, tableDetails.tableName, tableDetails.columnDefinitions);
}

bool portfolio::DatabaseComplexInterfaceImplementation::saveData(const db_manager::TableDetails& tableDetails,
                                                                 const db_manager::values_t& values) {
    return m_dbOrm.save(m_dbPath, tableDetails.tableName, tableDetails.columns, values);
}

bool portfolio::DatabaseComplexInterfaceImplementation::removeData(const db_manager::TableDetails& tableDetails,
                                                                   const std::string& uniqueId) {
    return m_dbOrm.remove(m_dbPath, tableDetails.tableName, uniqueId, tableDetails.primaryKeyName);
}

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const db_manager::TableDetails& tableDetails,
                                                                   const std::string& uniqueId,
                                                                   const std::string& itemName,
                                                                   const std::string& newValue) {
    return m_dbOrm.update(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName,
                          uniqueId, itemName, newValue);
}

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const db_manager::TableDetails& tableDetails,
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

bool portfolio::DatabaseComplexInterfaceImplementation::updateData(const db_manager::TableDetails& tableDetails,
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
                                        const db_manager::TableDetails& tableDetails,
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

bool portfolio::DatabaseComplexInterfaceImplementation::listData(const db_manager::TableDetails& tableDetails,
                                                                 db_manager::values_t& result) {
    return m_dbOrm.list(m_dbPath, tableDetails.tableName, tableDetails.primaryKeyName, result);
}

bool portfolio::DatabaseComplexInterfaceImplementation::listData(const db_manager::TableDetails& tableDetails,
                  std::vector<std::unordered_map<std::string, std::string>>& result) {
    return m_dbOrm.list(m_dbPath, tableDetails.tableName, result);
}

portfolio::DatabaseComplexInvestments::DatabaseComplexInvestments(
    const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
    const std::string& dbPath) {
    m_dbInterface = std::make_shared<DatabaseComplexInterfaceImplementation>(dbStrategy, dbPath);
    m_investmentsTable = setUpPredefinedInvestmentsTable();
    m_transactionsTable = setUpPredefinedTransactionsTable();
}

bool portfolio::DatabaseComplexInvestments::createTables() {
    return m_dbInterface->createTable(m_investmentsTable) &&
           m_dbInterface->createTable(m_transactionsTable);
}

bool portfolio::DatabaseComplexInvestments::saveComplexInvestment(const ComplexInvestment& cInvestment) {
    db_manager::values_t values {"'" + cInvestment.getId() + "'",
                                 "'" + cInvestment.getInvestment().getName() + "'",
                                 "'" + cInvestment.getInvestment().getTicker() + "'",
                                 std::to_string(cInvestment.getInvestment().getPurchasePrice()),
                                 std::to_string(cInvestment.getInvestment().getQuantity())};
    bool retVal = m_dbInterface->saveData(m_investmentsTable, values);
    values.clear();
    values = {"'" + cInvestment.getId() + "'"
              "'" + cInvestment.getTransaction().m_datetime.getDateTimeString() + "'",
              std::to_string(cInvestment.getTransaction().m_fees),
              "'" + cInvestment.getTransaction().currency_to_str() + "'",
              std::to_string(cInvestment.getTransaction().m_conversion_fees),
              std::to_string(cInvestment.getCurrencyConversionRate())};
    return retVal && m_dbInterface->saveData(m_transactionsTable, values);
}

bool portfolio::DatabaseComplexInvestments::removeComplexInvestment(const std::string& id) {
    return m_dbInterface->removeData(m_transactionsTable, id) &&
           m_dbInterface->removeData(m_investmentsTable, id);
}

bool portfolio::DatabaseComplexInvestments::updateInvestment(const ComplexInvestment& cInvestment) {
    db_manager::values_t values {"'" + cInvestment.getId() + "'",
                                 "'" + cInvestment.getInvestment().getName() + "'",
                                 "'" + cInvestment.getInvestment().getTicker() + "'",
                                 std::to_string(cInvestment.getInvestment().getPurchasePrice()),
                                 std::to_string(cInvestment.getInvestment().getQuantity())};
    bool retVal = m_dbInterface->updateData(m_investmentsTable, cInvestment.getId(), values);
    values.clear();
    values = {"'" + cInvestment.getId() + "'"
              "'" + cInvestment.getTransaction().m_datetime.getDateTimeString() + "'",
              std::to_string(cInvestment.getTransaction().m_fees),
              "'" + cInvestment.getTransaction().currency_to_str() + "'",
              std::to_string(cInvestment.getTransaction().m_conversion_fees),
              std::to_string(cInvestment.getCurrencyConversionRate())};
    return retVal && m_dbInterface->updateData(m_transactionsTable, cInvestment.getId(), values);
}

bool portfolio::DatabaseComplexInvestments::getInvestment(const std::string& id,
                                                          ComplexInvestment& cInvestment) {
    std::unordered_map<std::string, std::string> retData;
    Transaction transaction;
    double_t currency_rate;
    Investment investment;
    auto retVal = m_dbInterface->getData(m_investmentsTable, id, retData);
    if (retVal) {
        retVal = extractInvestmentFromData(investment, retData);
    }
    retData.clear();
    retVal &= m_dbInterface->getData(m_transactionsTable, id, retData);
    if (retVal) {
        retVal = extractTransactionFromData(transaction, currency_rate, retData);
    }
    if (retVal) {
        cInvestment.setInvestment(investment);
        cInvestment.setId(id);
        cInvestment.setCurrencyConversionRate(currency_rate);
        cInvestment.setTransaction(transaction);
    }
    return retVal;
}

bool portfolio::DatabaseComplexInvestments::listInvestment(std::vector<ComplexInvestment>& cInvestments) {
    std::vector<std::unordered_map<std::string, std::string>> retData;
    std::unordered_map<std::string, Investment> investments;
    auto retVal = m_dbInterface->listData(m_investmentsTable, retData);
    if (retVal) {
        retVal = extractInvestmentsFromData(investments, retData);
    }
    retData.clear();
    std::unordered_map<std::string, std::pair<Transaction, double_t>> transactions;
    retVal &= m_dbInterface->listData(m_transactionsTable, retData);
    if (retVal) {
        retVal = extractTransactionsFromData(transactions, retData);
    }
    if (retVal) retVal = (investments.size() == transactions.size());
    if (retVal) {
        for (const auto& investment : investments) {
            if (transactions.find(investment.first) != transactions.end()) {
                ComplexInvestment tmpCInv;
                tmpCInv.setCurrencyConversionRate(transactions.at(investment.first).second);
                tmpCInv.setId(investment.first);
                tmpCInv.setInvestment(investment.second);
                tmpCInv.setTransaction(transactions.at(investment.first).first);
                cInvestments.push_back(tmpCInv);
            }
        }
    }
    return retVal;
}

db_manager::TableDetails portfolio::DatabaseComplexInvestments::setUpPredefinedInvestmentsTable() {
    db_manager::TableDetails retTable;
    db_manager::columns_t columns = {"id", "name", "ticker", "purchase_price", "quantity"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"id", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"name", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"purchase_price", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"quantity", "REAL NOT NULL"});
    retTable.tableName = std::string("investments");
    retTable.primaryKeyName = std::string("id");
    retTable.columns = columns;
    retTable.columnDefinitions = columnDefinitions;
    return retTable;
}

db_manager::TableDetails portfolio::DatabaseComplexInvestments::setUpPredefinedTransactionsTable() {
    db_manager::TableDetails retTable;
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
    retTable.tableName = std::string("transactions");
    retTable.primaryKeyName = std::string("id");
    retTable.columns = columns;
    retTable.columnDefinitions = columnDefinitions;
    return retTable;
}

bool portfolio::DatabaseComplexInvestments::extractInvestmentFromData(Investment& investment,
                            const std::unordered_map<std::string, std::string>& retData) {
    std::array<bool, 4> validArr {false};
    for (const auto& column : m_investmentsTable.columns) {
        if (column == "name") {
            investment.setName(retData.at(column));
            validArr[0] = true;
        }
        else if (column == "ticker") {
            investment.setTicker(retData.at(column));
            validArr[1] = true;
        }
        else if (column == "purchase_price") {
            investment.setPurchasePrice(std::stod(retData.at(column)));
            validArr[2] = true;
        }
        else if (column == "quantity") {
            investment.setQuantity(std::stod(retData.at(column)));
            validArr[3] = true;
        }
    }
    auto retVal = true;
    for (const auto& valid : validArr) {
        retVal &= valid;
    }
    return retVal;
}

bool portfolio::DatabaseComplexInvestments::extractTransactionFromData(Transaction& transaction,
                            double_t& currency_rate,
                            const std::unordered_map<std::string, std::string>& retData) {
    std::array<bool, 5> validArr {false};
    for (const auto& column : m_transactionsTable.columns) {
        if (column == "date") {
            transaction.m_datetime.setDateTimeFromString(retData.at(column));
            validArr[0] = true;
        }
        else if (column == "fees") {
            transaction.m_fees = std::stod(retData.at(column));
            validArr[1] = true;
        }
        else if (column == "currency") {
            auto curr = retData.at(column);
            transaction.str_to_currency(curr);
            validArr[2] = true;
        }
        else if (column == "currency_conversion_fees") {
            transaction.m_conversion_fees = std::stod(retData.at(column));
            validArr[3] = true;
        }
        else if (column == "currency_conversion_rate") {
            currency_rate = std::stod(retData.at(column));
            validArr[4] = true;
        }
    }
    auto retVal = true;
    for (const auto& valid : validArr) {
        retVal &= valid;
    }
    return retVal;
}
bool portfolio::DatabaseComplexInvestments::extractInvestmentsFromData(
                std::unordered_map<std::string, Investment>& investments,
                const std::vector<std::unordered_map<std::string, std::string>>& retData) {
    bool retVal = false;
    for (const auto& retMap : retData) {
        bool mapSetForId = false;
        bool mapSetForName = false;
        bool mapSetForTicker = false;
        bool mapSetForPrice = false;
        bool mapSetForQuantity = false;
        std::string id, name, ticker, price, quantity;
        for (const auto& column : m_investmentsTable.columns) {
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
            investments.at(id) = Investment(name, ticker, std::stod(price), std::stod(price), std::stod(quantity));
            retVal = true;
        }
    }
    return retVal;
}

bool portfolio::DatabaseComplexInvestments::extractTransactionsFromData(
                std::unordered_map<std::string, std::pair<Transaction, double_t>>& transactions,
                const std::vector<std::unordered_map<std::string, std::string>>& retData) {
    bool retVal = false;
    for (const auto& retMap : retData) {
        bool mapSetForId = false;
        bool mapSetForDate = false;
        bool mapSetForFees = false;
        bool mapSetForCurrency = false;
        bool mapSetForCCF = false;
        bool mapSetForCCR = false;
        std::string id, date, fees, currency, ccf, ccr;
        for (const auto& column : m_transactionsTable.columns) {
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
            retVal = true;
        }
    }
    return retVal;
}
