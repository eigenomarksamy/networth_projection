#ifndef PORTFOLIO_APPL_HPP_
#define PORTFOLIO_APPL_HPP_

#include "portfolio.hpp"

namespace portfolio {

void executePortfolioManagement(portfolio::Portfolio& portfolio);

void executeMultiPortfolioManagement(portfolio::PortfolioManager& portfolio_mngr);

void savePortfolio(const Portfolio& portfolio, const std::string& filename);

bool loadPortfolio(Portfolio& portfolio, const std::string& filename);

} // namespace portfolio

#endif /* PORTFOLIO_APPL_HPP_ */