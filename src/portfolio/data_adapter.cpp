#include "generator_portfolio.hpp"

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