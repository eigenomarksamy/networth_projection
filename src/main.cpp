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

static void executePortfolioMgr();
static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const std::string& mortgage_caculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataNetworthProjector& confInputNetw,
                              const InputDataMortgageCalculator& confInputMrtg);

static void executePortfolioMgr() {
    portfolio::PortfolioMgrCfg portfolioInput;
    if (!portfolio::setUpPortfolioCfg(portfolioInput)) return;
    std::string tableName = "investments";
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
                                   tableName,
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
            valid = portfolio::updatePortfoliosDb(portfolio_manager, portfolioInput.db_dir,
                                                  tableName, portfolioInput.auto_save, dbStrategy);
        }
    }
}

static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const std::string& mortgage_caculator_path_output,
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
                                mortgage_caculator_path_output,
                                mortgage_calculator_path_input);
    }
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    std::string networth_conf_file = "conf/input/networth.yml";
    std::string mortgage_conf_file = "conf/input/mortgage.yml";
    DirectoriesValues dirs;
    NetworthValues nw_cfg_values;
    MortgageValues mortg_cfg_values;
    InputDataNetworthProjector input_data_nw_from_yml;
    InputDataMortgageCalculator input_data_mortg_from_yml;
    bool dirCfgRet = false;
    if (resolveCfg(dir_conf_file, dirs)) {
        dirCfgRet = true;
    }
    bool netwCfgRet = false;
    if (resolveCfg(networth_conf_file, nw_cfg_values)) {
        convertNetworthYmlData(input_data_nw_from_yml, nw_cfg_values);
        netwCfgRet = true;
    }
    bool mortgCfgRet = false;
    if (resolveCfg(mortgage_conf_file, mortg_cfg_values)) {
        convertMortgageYmlData(input_data_mortg_from_yml, mortg_cfg_values);
        mortgCfgRet = true;
    }
    if (doesUserWantStaticProgram()) {
        executeStaticAppl(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
                          dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
                          input_data_nw_from_yml, input_data_mortg_from_yml);
    }
    if (getUserYesNo("portfolio manager mode")) {
        executePortfolioMgr();
    }
    return 0;
}