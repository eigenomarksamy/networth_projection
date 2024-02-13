#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include "utils.hpp"
#include "networth.hpp"
#include "mortgage.hpp"
#include "file_generator.hpp"
#include "portfolio_file.hpp"
#include "portfolio_appl.hpp"
#include "portfolio_cfg.hpp"
#include "portfolio.hpp"
#include "utils.hpp"
#include "kafka.hpp"
#include "input_factory.hpp"
#include "yml_prsr.hpp"
#include "activation.hpp"
#include "conf_resolver.hpp"
#include "appl_conf_types.hpp"
#include "logger.hpp"
#include "strategy_db.hpp"

static void executePortfolioMgr(const std::string& confFile);
static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const std::string& mortgage_calculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataNetworthProjector& confInputNetw,
                              const InputDataMortgageCalculator& confInputMrtg);

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

static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const std::string& mortgage_calculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataNetworthProjector& confInputNetw,
                              const InputDataMortgageCalculator& confInputMrtg) {
    InputDataContainer user_input;
    InputDataContainer confInput;
    confInput.mortgage_calculator = confInputMrtg;
    confInput.networth_projector = confInputNetw;
    getProgramSelector(user_input, confInput);
    if (user_input.specifier == InputDataContainer::Specifier::NETWORTH_INPUT
        || user_input.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        executeStaticComputation(user_input,
                                networth_projector_path_output,
                                networth_projector_path_input,
                                mortgage_calculator_path_output,
                                mortgage_calculator_path_input);
    }
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    executePortfolioMgr(dir_conf_file);
    return 0;
}