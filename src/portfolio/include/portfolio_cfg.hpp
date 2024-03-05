#ifndef PORTFOLIO_CFG_HPP_
#define PORTFOLIO_CFG_HPP_

#include <vector>
#include <string>
#include "utils.hpp"
#include "conf_resolver.hpp"

namespace portfolio {

#define DEFAULT_PORTFOLIO_NAME ("MyInvestmentPortfolio_" + getLocalDateTime())

struct PortfolioMgrCfg {
    bool load_all_portfolios = true;
    bool auto_save = true;
    bool auto_log = true;
    bool is_complex_investment = false;
    std::string portfolio_src = "db";
    std::string gen_dir = "gen/portfolios/";
    std::string db_dir = "db/test/portfolios/";
    std::string table_name = "investments";
    uint8_t low_latency_db_threshold = 10;
    std::vector<std::string> portfolio_list;
};

struct PortfolioMgrYmlDirs {
    std::shared_ptr<config_elm_t> generation;
    std::shared_ptr<config_elm_t> database;
    std::shared_ptr<config_elm_t> logs;
    std::shared_ptr<config_elm_t> text;
    std::shared_ptr<config_elm_t> configuration;
};

struct PortfolioMgrYmlVals {
    std::shared_ptr<config_elm_t> auto_save;
    std::shared_ptr<config_elm_t> auto_log;
    std::shared_ptr<config_elm_t> auto_load;
    std::shared_ptr<config_elm_t> source;
    std::shared_ptr<config_elm_t> table_name;
    std::shared_ptr<config_elm_t> low_latency_db_threshold;
    std::shared_ptr<config_elm_t> is_complex_investment;
};

bool resolveCfg(const std::string& confPath, PortfolioMgrYmlDirs& dirs);

bool resolveCfg(const std::string& confPath, PortfolioMgrYmlVals& vals);

void convertYmlData(PortfolioMgrCfg& inputs,
                    const PortfolioMgrYmlDirs& dirs);

enum class PortfolioCfgInputSource { NONE, DEFAULT, MANUAL_CMD, CONF_FILE, WEB_UI, APP_UI };

} // namespace portfolio

#endif /* PORTFOLIO_CFG_HPP_ */