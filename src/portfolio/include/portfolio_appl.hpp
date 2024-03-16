#ifndef PORTFOLIO_APPL_HPP_
#define PORTFOLIO_APPL_HPP_

#include "portfolio.hpp"
#include "portfolio_cfg.hpp"

namespace portfolio {

void executePortfolioManagement(Portfolio& portfolio);

void executeMultiPortfolioManagement(PortfolioManager& portfolio_mngr);

void executeTransactionalPortfolioManagement(TransactionalPortfolio& portfolio);

void executeMultiTransactionalPortfolioManagement(TransactionalPortfolioManager& portfolioMgr);

PortfolioCfgInputSource setPortfolioInputSource();

void setUpPortfolioManually(PortfolioMgrCfg& conf);

void setUpPortfolioConfigurationFile(PortfolioMgrCfg& conf, const std::string& dirFile);

bool setUpPortfolioCfg(PortfolioMgrCfg& conf, const std::string& dirFile);

} // namespace portfolio

#endif /* PORTFOLIO_APPL_HPP_ */