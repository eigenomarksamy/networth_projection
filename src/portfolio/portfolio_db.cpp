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

portfolio::DatabaseTransactionalImplementation::DatabaseTransactionalImplementation(
        const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy,
        const std::string& dbPath,
        const std::vector<std::string>& tableNames) 
  : m_dbStrategy(dbStrategy), m_dbPath(dbPath), m_tableNames(tableNames),
    m_dbOrm(db_manager::DatabaseORM(m_dbStrategy)) {
    
}

bool portfolio::DatabaseTransactionalImplementation::fillTableParameters(
                            const std::string& tableToFill, const db_manager::columns_t& columns,
                            const std::vector<db_manager::columnDefinition_t>& columnDefinitions) {
    auto idx = findIdx(m_tableNames, std::string(tableToFill));
    if (-1 != idx) {
        m_mapTableIdsColumns[idx] = columns;
        m_mapTableIdsColumnDefinitions[idx] = columnDefinitions;
        return true;
    }
    return false;
}

void portfolio::DatabaseTransactionalImplementation::defineInvestmentsTable() {
    db_manager::columns_t columns = {"id", "name", "ticker", "purchase_price", "quantity"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"id", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"name", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"purchase_price", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"quantity", "REAL NOT NULL"});
    fillTableParameters("investments", columns, columnDefinitions);
}

void portfolio::DatabaseTransactionalImplementation::defineMarketPricesTable() {
    db_manager::columns_t columns = {"ticker", "date", "exchange", "price"};
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"date", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"exchange", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"price", "REAL NOT NULL"});
    fillTableParameters("prices", columns, columnDefinitions);
}

void portfolio::DatabaseTransactionalImplementation::defineTransactionsTable() {
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
    fillTableParameters("transactions", columns, columnDefinitions);
}

bool portfolio::DatabaseTransactionalImplementation::createTable(const std::string& tableName) {
    auto idx = findIdx(m_tableNames, tableName);
    if (-1 != idx) {
        return m_dbOrm.createTable(m_dbPath, m_tableNames[idx], m_mapTableIdsColumnDefinitions[idx]);
    }
    return false;
}

bool portfolio::DatabaseTransactionalImplementation::saveInvestment(const ComplexInvestment& investment) {
    bool retVal = true;
    std::string tableName = "investments";
    uint16_t idx = -1;
    db_manager::values_t values {"'" + investment.getId() + "'",
                                 "'" + investment.getInvestment().getName() + "'",
                                 "'" + investment.getInvestment().getTicker() + "'",
                                 std::to_string(investment.getInvestment().getPurchasePrice()),
                                 std::to_string(investment.getInvestment().getQuantity())};
    idx = findIdx(m_tableNames, tableName);
    retVal &= (-1 != idx);
    if (retVal) {
        retVal &= m_dbOrm.save(m_dbPath, m_tableNames[idx], m_mapTableIdsColumns[idx], values);
    }
    tableName = "transactions";
    values.clear();
    std::string curr_str = (investment.getTransaction().m_currency == Transaction::Currency::USD) ? "USD" : "EUR";
    values = {"'" + investment.getId() + "'",
              "'" + investment.getTransaction().m_datetime.getDateTimeString() + "'",
              std::to_string(investment.getTransaction().m_fees),
              "'" + curr_str + "'",
              std::to_string(investment.getTransaction().m_conversion_fees),
              std::to_string(investment.getCurrencyConversionRate())};
    idx = findIdx(m_tableNames, tableName);
    retVal &= (-1 != idx);
    if (retVal) {
        retVal &= m_dbOrm.save(m_dbPath, m_tableNames[idx], m_mapTableIdsColumns[idx], values);
    }
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::savePrice(const std::string& ticker, const DateTime& date,
                                                               const std::string& exchange, const double_t price) {
    std::string tableName = "prices";
    auto idx = findIdx(m_tableNames, tableName);
    db_manager::values_t values {"'" + ticker + "'",
                                 "'" + date.getDateTimeString() + "'",
                                 "'" + exchange + "'",
                                 std::to_string(price)};
    if (-1 != idx) {
        return m_dbOrm.save(m_dbPath, m_tableNames[idx], m_mapTableIdsColumns[idx], values);
    }
    return false;
}

bool portfolio::DatabaseTransactionalImplementation::removeInvestment(const std::string& id) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::removePrice(const std::string& ticker) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::updateInvestmentQuantity(const std::string& id, const double_t quantity) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::updatePrice(const std::string& ticker, const double_t price) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::getInvestment(const std::string& id, ComplexInvestment& investment) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::getPrice(const std::string& ticker, double_t& price) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::listInvestments(std::vector<ComplexInvestment>& investments) {
    bool retVal = true;
    return retVal;
}

bool portfolio::DatabaseTransactionalImplementation::listPrices(std::map<std::string, double_t>& prices) {
    bool retVal = true;
    return retVal;
}
