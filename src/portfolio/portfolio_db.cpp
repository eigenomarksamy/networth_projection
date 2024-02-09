#include "portfolio_db.hpp"

portfolio::DatabaseInterfaceImplementation::DatabaseInterfaceImplementation(
                                                const std::shared_ptr<db_manager::DatabaseStrategy>& db_strategy,
                                                const std::string& dbPath,
                                                const std::string& tableName)
  : m_dbStrategy(db_strategy), m_dbPath(dbPath), m_tableName(tableName),
    m_dbOrm(db_manager::DatabaseORM(m_dbStrategy)) {
    m_columns = {"name", "ticker", "purchase_price", "quantity"};
}

bool portfolio::DatabaseInterfaceImplementation::createTable() {
    std::vector<db_manager::columnDefinition_t> columnDefinitions;
    columnDefinitions.push_back(db_manager::columnDefinition_t{"name", "TEXT NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"ticker", "TEXT PRIMARY KEY"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"purchase_price", "REAL NOT NULL"});
    columnDefinitions.push_back(db_manager::columnDefinition_t{"quantity", "INTEGER NOT NULL"});
    return m_dbOrm.createTable(m_dbPath, m_tableName, columnDefinitions);
}

bool portfolio::DatabaseInterfaceImplementation::saveInvestment(const Investment& investment) {
    db_manager::values_t values {"'" + investment.getName() + "'",
                                 "'" + investment.getTicker() + "'",
                                 std::to_string(investment.getPurchasePrice()),
                                 std::to_string(investment.getQuantity())};
    if (m_dbOrm.save(m_dbPath, m_tableName, m_columns, values))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) {
    if (m_dbOrm.update(m_dbPath, m_tableName, "'" + ticker + "'", "ticker", "quantity", std::to_string(quantity)))
        return true;
    return false;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentPrice(const std::string& ticker, const double_t price) {
    if (m_dbOrm.update(m_dbPath, m_tableName, "'" + ticker + "'", "ticker", "purchase_price", std::to_string(price)))
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
            auto retValI = std::stoi(retVal);
            investment.setQuantity(retValI);
        }
        else if ("purchase_price" == column && retFlag) {
            auto retValD = std::stod(retVal);
            investment.setPurchasePrice(retValD);
        }
        else if ("name" == column && retFlag) {
            investment.setName(retVal);
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