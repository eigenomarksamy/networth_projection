#ifndef PORTFOLIO_APPL_HPP_
#define PORTFOLIO_APPL_HPP_

#include "portfolio.hpp"
#include "portfolio_cfg.hpp"

namespace portfolio {

void executePortfolioManagement(portfolio::Portfolio& portfolio);

void executeMultiPortfolioManagement(portfolio::PortfolioManager& portfolio_mngr);

PortfolioCfgInputSource setPortfolioInputSource();

void setUpPortfolioManually(PortfolioMgrCfg& conf);

void setUpPortfolioConfigurationFile(PortfolioMgrCfg& conf, const std::string& dirFile);

bool setUpPortfolioCfg(PortfolioMgrCfg& conf, const std::string& dirFile);

} // namespace portfolio

#endif /* PORTFOLIO_APPL_HPP_ */