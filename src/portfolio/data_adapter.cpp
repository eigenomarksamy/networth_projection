#include "generator_portfolio.hpp"
#include "portfolio_appl.hpp"
#include "cmd_common.hpp"
#include "file_generator.hpp"

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