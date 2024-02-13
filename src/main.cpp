#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include "utils.hpp"
#include "file_generator.hpp"
#include "portfolio_file.hpp"
#include "portfolio_appl.hpp"
#include "portfolio_cfg.hpp"
#include "portfolio.hpp"
#include "utils.hpp"
#include "kafka.hpp"
#include "input_factory.hpp"
#include "yml_prsr.hpp"
#include "conf_resolver.hpp"
#include "logger.hpp"
#include "strategy_db.hpp"

static void executePortfolioMgr(const std::string& confFile);

static void executePortfolioMgr(const std::string& confFile) {
    portfolio::PortfolioMgrCfg portfolioInput;
    if (!portfolio::setUpPortfolioCfg(portfolioInput, confFile)) return;
    auto dbStrategy = std::make_shared<db_manager::SQLiteStrategy>();
    portfolio::PortfolioManager portfolio_manager;
    bool valid = false;
    if (portfolioInput.portfolio_src == "text"
        && getPortfolioFromFiles(portfolio_manager,
                                 portfolioInput.load_all_portfolios,
                                 portfolioInput.portfolio_list,
                                 portfolioInput.gen_dir)) {
        valid = true;
    }
    else if (portfolioInput.portfolio_src == "db"
             && getPortfolioFromDb(portfolio_manager,
                                   portfolioInput.load_all_portfolios,
                                   portfolioInput.portfolio_list,
                                   portfolioInput.db_dir,
                                   portfolioInput.table_name,
                                   dbStrategy)) {
            valid = true;
    }
    if (valid) {
        if (portfolioInput.auto_log) {
            auto portfolio_logger_ptr = std::make_shared<portfolio::PortfolioLogger>();
            portfolio_manager.setLoggerPtr(portfolio_logger_ptr);
        }
        portfolio::executeMultiPortfolioManagement(portfolio_manager);
        if (portfolioInput.portfolio_src == "text") {
            generatePortfolioOverview(portfolio_manager,
                                      portfolioInput.gen_dir,
                                      portfolioInput.gen_dir + "../portfolio_overview.txt",
                                      portfolioInput.auto_save);
        }
        else if (portfolioInput.portfolio_src == "db") {
            valid = portfolio::updatePortfoliosDb(portfolio_manager,
                                                  portfolioInput.db_dir,
                                                  portfolioInput.table_name,
                                                  portfolioInput.auto_save,
                                                  dbStrategy);
        }
    }
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    executePortfolioMgr(dir_conf_file);
    return 0;
}