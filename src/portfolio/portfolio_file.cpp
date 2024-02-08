#include "cmd_common.hpp"
#include "file_generator.hpp"
#include "portfolio_file.hpp"

std::vector<std::string> portfolio::DataAdapter::generatePortfolioLines(const portfolio::Portfolio& portfolioObj) {
    std::vector<std::string> outLines;
    const std::vector<Investment>& investments = portfolioObj.getInvestments();
    outLines.push_back(portfolioObj.getName());
    for (const Investment& investment : investments) {
        outLines.push_back(investment.getName() + "," + investment.getTicker()
                           + "," + std::to_string(investment.getPurchasePrice())
                           + ","+ std::to_string(investment.getQuantity()));
    }
    return outLines;
}

std::vector<std::string> portfolio::DataAdapter::generatePortfolioLines(const portfolio::PortfolioManager& portfolioMgrObj) {
    std::vector<std::string> retGenLines;
    for (auto i = 0; i < portfolioMgrObj.getNumPortfolios(); ++i) {
        portfolio::Portfolio& portfolio = portfolioMgrObj.getPortfolio(i);
        auto tmpPortfolioLines = generatePortfolioLines(portfolio);
        retGenLines.insert(retGenLines.end(),
                           std::make_move_iterator(tmpPortfolioLines.begin()),
                           std::make_move_iterator(tmpPortfolioLines.end()));
    }
    return retGenLines;
}

void portfolio::generatePortfolioFiles(const portfolio::PortfolioManager& portfolioMgr,
                                       const std::string& directory,
                                       const bool autoSave) {
    for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
        auto portfolio = portfolioMgr.getPortfolio(i);
        std::string question = "save portfolio " + portfolio.getName();
        if (autoSave || getUserYesNo(question)) {
            portfolio::savePortfolio(portfolio, directory + portfolio.getName());
        }
    }
}

void portfolio::generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                                       const std::string& directory,
                                       const bool autoSave) {
    if (autoSave || getUserYesNo("save portfolio " + portfolio.getName())) {
        portfolio::savePortfolio(portfolio, directory + portfolio.getName());
    }
}

void portfolio::generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                                          const std::string& directory,
                                          const std::string& outputFile,
                                          const bool autoSave) {
    auto portfolioTxt = portfolio::DataAdapter::generatePortfolioLines(portfolio);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolio, directory, autoSave);
}

void portfolio::generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                                          const std::string& directory,
                                          const std::string& outputFile,
                                          const bool autoSave) {
    auto portfolioTxt = portfolio::DataAdapter::generatePortfolioLines(portfolioMgr);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolioMgr, directory, autoSave);
}

void portfolio::savePortfolio(const Portfolio& portfolio, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << portfolio.getName() << std::endl;
        const std::vector<Investment>& investments = portfolio.getInvestments();
        for (const Investment& investment : investments) {
            file << investment.getName() << "," << investment.getTicker() << ","
                 << investment.getPurchasePrice() << "," << investment.getQuantity() << std::endl;
        }
        std::cout << "Portfolio saved to " << filename << std::endl;
    }
    else {
        std::cout << "Unable to open file for saving portfolio." << std::endl;
    }
    file.close();
}

bool portfolio::savePortfolioDb(const std::string& fileName,
                                const std::string& tableName,
                                const Portfolio& portfolio,
                                const bool is_new,
                                const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy) {
    bool retVal = true;
    auto dbInterface = DatabaseInterfaceImplementation(dbStrategy, fileName, tableName);
    if (is_new) {
        retVal &= dbInterface.createTable();
    }
    if (!retVal) return false;
    for (const auto& investment : portfolio.getInvestments()) {
        retVal &= dbInterface.saveInvestment(investment);
    }
    return retVal;
}

bool portfolio::updatePortfoliosDb(const PortfolioManager& portfolioMgr,
                                   const std::string& directory,
                                   const std::string& tableName,
                                   const bool autoSave,
                                   const std::shared_ptr<db_manager::DatabaseStrategy> &dbStrategy) {
    bool retVal = true;
    auto filesInDir = getFileNames(directory);
    for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
        auto portfolio = portfolioMgr.getPortfolio(i);
        std::string question = "save portfolio " + portfolio.getName();
        if (autoSave || getUserYesNo(question)) {
            std::string fileName = directory + portfolio.getName() + ".db";
            bool isNew = true;
            if (std::find(filesInDir.begin(), filesInDir.end(), fileName) != filesInDir.end()) {
                isNew = false;
            }
            retVal &= portfolio::savePortfolioDb(fileName, tableName, portfolio, isNew, dbStrategy);
        }
    }
    return retVal;
}

bool portfolio::loadPortfolio(Portfolio& portfolio, const std::string& filename) {
    bool status = true;
    std::ifstream file(filename);
    portfolio.clearInvestments();
    if (file.is_open()) {
        std::string name, ticker, line;
        double_t purchasePrice;
        uint32_t quantity;
        std::getline(file, name);
        portfolio.setName(name);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::getline(iss, name, ',');
            std::getline(iss, ticker, ',');
            iss >> purchasePrice;
            iss.ignore();
            iss >> quantity;
            Investment investment(name, ticker, purchasePrice, quantity);
            portfolio.addInvestment(investment);
        }
        std::cout << "Portfolio loaded from " << filename << std::endl;
    }
    else {
        status = false;
        std::cout << "Unable to open file for load portfolio.\n";
    }
    file.close();
    return status;
}

bool portfolio::getPortfolioFromFiles(portfolio::Portfolio& portfolio,
                           const std::string& name,
                           const std::string& directory) {
    return portfolio::loadPortfolio(portfolio, directory + name);
}

bool portfolio::getPortfolioFromFiles(portfolio::PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios,
                           const std::string& directory) {
    bool status = true;
    std::string directoryPath = directory;
    std::vector<std::string> names;
    if (load_all_portfolios)
        names = getFileNames(directoryPath);
    else
        names = list_portfolios;
    for (const auto& name : names) {
        portfolio::Portfolio portfolio;
        if (getPortfolioFromFiles(portfolio, name, directory)) {
            portfolioMgr.addPortfolio(portfolio);
            status &= true;
        }
        else {
            status = false;
        }
    }
    return status;
}

bool portfolio::loadPortfolioDb(Portfolio& portfolio,
                                const std::string& filename,
                                const std::string& tableName,
                                const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy) {
    bool retVal;
    auto dbInterface = DatabaseInterfaceImplementation(dbStrategy, filename, tableName);
    std::vector<Investment> investments;
    retVal = dbInterface.listInvestments(investments);
    if (retVal && !investments.empty()) {
        portfolio.clearInvestments();
        portfolio.addInvestments(investments);
    }
    return retVal;
}

bool portfolio::getPortfolioFromDb(portfolio::Portfolio& portfolio,
                                   const std::string& name,
                                   const std::string& directory,
                                   const std::string& tableName,
                                   const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy) {
    if (portfolio::loadPortfolioDb(portfolio, directory + name, tableName, dbStrategy)) {
        auto pos = name.find(".");
        auto act_name = name.substr(0, pos);
        portfolio.setName(act_name);
        return true;
    }
    return false;
}

bool portfolio::getPortfolioFromDb(portfolio::PortfolioManager& portfolioMgr,
                                   const bool load_all_portfolios,
                                   const std::vector<std::string>& list_portfolios,
                                   const std::string& directory,
                                   const std::string& tableName,
                                   const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy) {
    bool status = true;
    std::string directoryPath = directory;
    std::vector<std::string> names;
    if (load_all_portfolios)
        names = getFileNames(directoryPath);
    else {
        for (const auto& lp : list_portfolios) {
            names.push_back(lp + ".db");
        }
    }
    for (const auto& name : names) {
        portfolio::Portfolio portfolio;
        dbStrategy->clearResults();
        if (getPortfolioFromDb(portfolio, name, directory, tableName, dbStrategy)) {
            portfolioMgr.addPortfolio(portfolio);
            status &= true;
        }
        else {
            status = false;
        }
    }
    return status;
}