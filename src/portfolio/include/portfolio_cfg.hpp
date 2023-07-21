#ifndef PORTFOLIO_CFG_HPP_
#define PORTFOLIO_CFG_HPP_

#include <vector>
#include <string>
#include "utils.hpp"

namespace portfolio {

#define DEFAULT_PORTFOLIO_NAME ("MyInvestmentPortfolio_" + getLocalDateTime())

struct PortfolioMgrCfg {
    bool is_new = true;
    bool is_multi_prtfolio = true;
    bool load_all_portfolios = true;
    bool auto_save = true;
    std::string name = DEFAULT_PORTFOLIO_NAME;
    std::string gen_dir = "gen/portfolios/";
    std::vector<std::string> portfolio_list;
};

} // namespace portfolio

#endif /* PORTFOLIO_CFG_HPP_ */