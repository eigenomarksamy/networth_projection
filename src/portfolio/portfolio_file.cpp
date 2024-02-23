#include <filesystem>
#include "cmd_common.hpp"
#include "file_generator.hpp"
#include "portfolio_file.hpp"

namespace portfolio {
static bool updatePortfoliosDbLowLatency(const PortfolioManager& portfolioMgrOg,
                                         const PortfolioManager& portfolioMgr,
                                         const std::string& directory,
                                         const std::string& tableName,
                                         const bool autoSave,
                                         const std::shared_ptr<db_manager::DatabaseStrategy> &dbStrategy);
}

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
    std::vector<Investment> existingInvestments;
    std::vector<std::string> existingInvestmentsTickers;
    auto dbInterface = DatabaseInterfaceImplementation(dbStrategy, fileName, tableName);
    if (is_new) {
        retVal &= dbInterface.createTable();
        if (!retVal) return false;
        auto cachedInvestments = portfolio.getInvestments();
        for (const auto& investment : cachedInvestments) {
            retVal &= dbInterface.saveInvestment(investment);
        }
    }
    else {
        retVal &= dbInterface.listInvestments(existingInvestments);
        for (const auto& investment : existingInvestments) {
            existingInvestmentsTickers.push_back(investment.getTicker());
        }
        for (const auto& investment : portfolio.getInvestments()) {
            if (std::find(existingInvestmentsTickers.begin(),
                        existingInvestmentsTickers.end(),
                        investment.getTicker()) != existingInvestmentsTickers.end()) {
                Investment existingInvestment;
                retVal &= dbInterface.getInvestment(investment.getTicker(), existingInvestment);
                if (existingInvestment.getQuantity() != investment.getQuantity()) {
                    retVal &= dbInterface.updateInvestmentQuantity(investment.getTicker(),
                                                                   investment.getQuantity());
                }
                if (existingInvestment.getPurchasePrice() != investment.getPurchasePrice()) {
                    retVal &= dbInterface.updateInvestmentPurchasePrice(investment.getTicker(),
                                                                        investment.getPurchasePrice());
                }
                if (existingInvestment.getCurrentPrice() != investment.getCurrentPrice()) {
                    retVal &= dbInterface.updateInvestmentCurrentPrice(investment.getTicker(),
                                                                       investment.getCurrentPrice());
                }
            }
            else {
                retVal &= dbInterface.saveInvestment(investment);
            }
        }
    }
    return retVal;
}

static bool portfolio::updatePortfoliosDbLowLatency(const PortfolioManager& portfolioMgrOg,
                                                    const PortfolioManager& portfolioMgr,
                                                    const std::string& directory,
                                                    const std::string& tableName,
                                                    const bool autoSave,
                                                    const std::shared_ptr<db_manager::DatabaseStrategy> &dbStrategy) {
    bool retVal = true;
    auto modified_num_portfolios = portfolioMgr.getNumPortfolios();
    auto original_num_portfolios = portfolioMgrOg.getNumPortfolios();
    std::vector<bool> matched(original_num_portfolios);
    for (auto&& m : matched) m = false;
    bool foundModifiedMatch = false;
    for (auto i = 0; i < modified_num_portfolios; ++i) {
        for (auto j = 0; j < original_num_portfolios; ++j) {
            auto original_name = portfolioMgrOg.getPortfolio(j).getName();
            auto modified_name = portfolioMgr.getPortfolio(i).getName();
            if (original_name == modified_name) {
                foundModifiedMatch = true;
                matched.at(j) = true;
                auto existing_portfolio = portfolioMgrOg.getPortfolio(j);
                auto cached_portfolio = portfolioMgr.getPortfolio(i);
                if (cached_portfolio != existing_portfolio) {
                    auto cached_investments = cached_portfolio.getInvestments();
                    auto existing_investments = existing_portfolio.getInvestments();
                    std::string fileName = portfolioMgr.getPortfolio(i).getName() + ".db";
                    std::string fullFileName = directory + fileName;
                    auto dbInterface = DatabaseInterfaceImplementation(dbStrategy, fileName, tableName);
                    std::vector<Investment> investmentsToRemove;
                    std::copy_if(existing_investments.begin(), existing_investments.end(),
                                std::back_inserter(investmentsToRemove),
                                [&](const Investment& existing_investment) {
                                    return std::find(cached_investments.begin(), cached_investments.end(),
                                                    existing_investment) == cached_investments.end();
                                });
                    for (const auto& investmentToRemove : investmentsToRemove) {
                        retVal &= dbInterface.removeInvestment(investmentToRemove.getTicker());
                    }
                    for (const auto& cached_investment : cached_investments) {
                        auto it = std::find_if(existing_investments.begin(), existing_investments.end(),
                                               [&](const auto& existing_investment) {
                                                   return existing_investment.getTicker() == cached_investment.getTicker();
                                               });
                        if (it != existing_investments.end()) {
                            if (it->getQuantity() != cached_investment.getQuantity()) {
                                dbInterface.updateInvestmentQuantity(it->getTicker(), cached_investment.getQuantity());
                            }
                            if (it->getPurchasePrice() != cached_investment.getPurchasePrice()) {
                                dbInterface.updateInvestmentPurchasePrice(it->getTicker(), cached_investment.getPurchasePrice());
                            }
                            if (it->getCurrentPrice() != cached_investment.getCurrentPrice()) {
                                dbInterface.updateInvestmentCurrentPrice(it->getTicker(), cached_investment.getCurrentPrice());
                            }
                        }
                        else {
                            retVal &= dbInterface.saveInvestment(cached_investment);
                        }
                    }
                }
            }
            else {
                break;
            }
        }
        if (!foundModifiedMatch) {
            std::string fileName = portfolioMgr.getPortfolio(i).getName() + ".db";
            std::string fullFileName = directory + fileName;
            auto dbInterface = DatabaseInterfaceImplementation(dbStrategy, fileName, tableName);
            retVal &= dbInterface.createTable();
            if (!retVal) return false;
            auto cachedInvestments = portfolioMgr.getPortfolio(i).getInvestments();
            for (const auto& investment : cachedInvestments) {
                retVal &= dbInterface.saveInvestment(investment);
            }
        }
        foundModifiedMatch = false;
    }
    for (auto i = 0; i < original_num_portfolios; ++i) {
        if (!matched.at(i)) {
            std::string searchFileName = portfolioMgrOg.getPortfolio(i).getName() + ".db";
            try {
                if (std::filesystem::remove(searchFileName)) retVal &= true;
                else retVal = false;
            }
            catch(const std::filesystem::filesystem_error& err) {
                std::cerr << "FileSystem Error: " << err.what() << std::endl;
            }
        }
    }
    return true;
}

bool portfolio::updatePortfoliosDb(const PortfolioManager& portfolioMgrOg,
                                   const PortfolioManager& portfolioMgr,
                                   const std::string& directory,
                                   const std::string& tableName,
                                   const bool autoSave,
                                   const uint8_t low_latency_threshold,
                                   const std::shared_ptr<db_manager::DatabaseStrategy> &dbStrategy) {
    bool retVal = true;
    if (portfolioMgr.getNumPortfolios() > low_latency_threshold
        || portfolioMgrOg.getNumPortfolios() > low_latency_threshold) {
        retVal &= updatePortfoliosDbLowLatency(portfolioMgrOg, portfolioMgr,
                                               directory, tableName,
                                               autoSave, dbStrategy);
    }
    else {
        auto filesInDir = getFileNames(directory);
        for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
            auto portfolio = portfolioMgr.getPortfolio(i);
            std::string question = "save portfolio " + portfolio.getName();
            if (autoSave || getUserYesNo(question)) {
                std::string fileName = portfolio.getName() + ".db";
                std::string fullFileName = directory + fileName;
                bool isNew = true;
                if (std::find(filesInDir.begin(), filesInDir.end(), fileName) != filesInDir.end()) {
                    isNew = false;
                }
                retVal &= portfolio::savePortfolioDb(fullFileName, tableName, portfolio, isNew, dbStrategy);
            }
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
        double_t currentPrice = 0;
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
            Investment investment(name, ticker, purchasePrice, currentPrice, quantity);
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