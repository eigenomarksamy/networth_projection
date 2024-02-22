#ifndef PORTFOLIO_FILE_HPP_
#define PORTFOLIO_FILE_HPP_

#include <vector>
#include <string>
#include <memory>
#include "portfolio.hpp"
#include "portfolio_db.hpp"

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

bool savePortfolioDb(const std::string& fileName,
                     const std::string& tableName,
                     const Portfolio& portfolio,
                     const bool is_new,
                     const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy);

bool updatePortfoliosDb(const PortfolioManager& portfolioMgrOg,
                        const PortfolioManager& portfolioMgr,
                        const std::string& directory,
                        const std::string& tableName,
                        const bool autoSave,
                        const std::shared_ptr<db_manager::DatabaseStrategy> &dbStrategy);

bool loadPortfolio(Portfolio& portfolio, const std::string& filename);

bool getPortfolioFromFiles(portfolio::Portfolio& portfolio,
                           const std::string& name,
                           const std::string& directory);

bool getPortfolioFromFiles(portfolio::PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios,
                           const std::string& directory);

bool loadPortfolioDb(Portfolio& portfolio,
                     const std::string& filename,
                     const std::string& tableName,
                     const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy);

bool getPortfolioFromDb(portfolio::Portfolio& portfolio,
                        const std::string& name,
                        const std::string& directory,
                        const std::string& tableName,
                        const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy);

bool getPortfolioFromDb(portfolio::PortfolioManager& portfolio,
                        const bool load_all_portfolios,
                        const std::vector<std::string>& list_portfolios,
                        const std::string& directory,
                        const std::string& tableName,
                        const std::shared_ptr<db_manager::DatabaseStrategy>& dbStrategy);

} // namespace portfolio

#endif /* PORTFOLIO_FILE_HPP_ */