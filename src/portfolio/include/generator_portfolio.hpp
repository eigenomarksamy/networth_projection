#ifndef GENERATOR_PORTFOLIO_HPP_
#define GENERATOR_PORTFOLIO_HPP_

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

} // namespace portfolio

#endif /* GENERATOR_PORTFOLIO_HPP_ */