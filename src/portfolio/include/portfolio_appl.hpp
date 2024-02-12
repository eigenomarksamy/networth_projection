#ifndef PORTFOLIO_APPL_HPP_
#define PORTFOLIO_APPL_HPP_

#include "portfolio.hpp"
#include "portfolio_cfg.hpp"

namespace portfolio {

void executePortfolioManagement(portfolio::Portfolio& portfolio);

void executeMultiPortfolioManagement(portfolio::PortfolioManager& portfolio_mngr);

PortfolioCfgInputSource setPortfolioInputSource();

void setUpPortfolioManually(PortfolioMgrCfg& conf);

void setUpPortfolioConfigurationFile(PortfolioMgrCfg& conf);

bool setUpPortfolioCfg(PortfolioMgrCfg& conf);

} // namespace portfolio

#endif /* PORTFOLIO_APPL_HPP_ */