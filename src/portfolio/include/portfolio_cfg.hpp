#ifndef PORTFOLIO_CFG_HPP_
#define PORTFOLIO_CFG_HPP_

#include <vector>
#include <string>
#include "utils.hpp"

namespace portfolio {

#define DEFAULT_PORTFOLIO_NAME ("MyInvestmentPortfolio_" + getLocalDateTime())

struct PortfolioMgrCfg {
    bool load_all_portfolios = true;
    bool auto_save = true;
    bool auto_log = true;
    std::string portfolio_src = "db";
    std::string gen_dir = "gen/portfolios/";
    std::string db_dir = "db/portfolios/";
    std::vector<std::string> portfolio_list;
};

enum class PortfolioCfgInputSource { NONE, DEFAULT, MANUAL_CMD, CONF_FILE, WEB_UI, APP_UI };

} // namespace portfolio

#endif /* PORTFOLIO_CFG_HPP_ */