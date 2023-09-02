#ifndef PORTFOLIO_FILE_HPP_
#define PORTFOLIO_FILE_HPP_

#include <vector>
#include <string>
#include "portfolio.hpp"

namespace portfolio {

class DataAdapter {

public:

    DataAdapter () { }

    ~DataAdapter() { }

    static std::vector<std::string> generatePortfolioLines(const portfolio::Portfolio& portfolioObj);

    static std::vector<std::string> generatePortfolioLines(const portfolio::PortfolioManager& portfolioMgrObj);
};

void generatePortfolioFiles(const portfolio::PortfolioManager& portfolioMgr,
                            const std::string& directory,
                            const bool autoSave);

void generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                            const std::string& directory,
                            const bool autoSave);

void generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave);

void generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave);

void savePortfolio(const Portfolio& portfolio, const std::string& filename);

void savePortfolioDb(const Portfolio& portfolio, const std::string& filename);

void generatePortfolioDb(const portfolio::Portfolio& portfolioMgr,
                         const std::string& directory,
                         const bool autoSave);

void generatePortfolioDb(const portfolio::PortfolioManager& portfolioMgr,
                         const std::string& directory,
                         const bool autoSave);

bool loadPortfolio(Portfolio& portfolio, const std::string& filename);

bool getPortfolioFromFiles(portfolio::Portfolio& portfolio,
                           const std::string& name,
                           const std::string& directory);

bool getPortfolioFromFiles(portfolio::PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios,
                           const std::string& directory);

bool loadPortfolioDb(Portfolio& portfolio, const std::string& filename);

bool getPortfolioFromDb(portfolio::Portfolio& portfolio,
                        const std::string& name,
                        const std::string& directory);

bool getPortfolioFromDb(portfolio::PortfolioManager& portfolio,
                        const bool load_all_portfolios,
                        const std::vector<std::string>& list_portfolios,
                        const std::string& directory);

} // namespace portfolio

#endif /* PORTFOLIO_FILE_HPP_ */