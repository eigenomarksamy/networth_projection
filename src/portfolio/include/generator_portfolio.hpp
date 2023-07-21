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

} // namespace portfolio

#endif /* GENERATOR_PORTFOLIO_HPP_ */