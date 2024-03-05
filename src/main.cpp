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

enum APPLICATION_IDX {
    PORTFOLIO_MANAGER = 0,
    MORTGAGE_CALCULATOR,
    NETWORTH_PROJECTOR,
    LENGTH
};

static std::array<bool, APPLICATION_IDX::LENGTH> resolveCfgAppSelector(const std::string& cfgFile);
static void executePortfolioMgr(const std::string& confFile);
static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const std::string& mortgage_calculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataNetworthProjector& confInputNetw,
                              const InputDataMortgageCalculator& confInputMrtg);
static void executeStaticAppl(const std::string& mortgage_calculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataMortgageCalculator& confInputMrtg);
static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const InputDataNetworthProjector& confInputNetw);

static void executePortfolioMgr(const std::string& confFile) {
    portfolio::PortfolioMgrCfg portfolioInput;
    if (!portfolio::setUpPortfolioCfg(portfolioInput, confFile)) return;
    if (portfolioInput.is_complex_investment) {
        portfolio::TransactionalPortfolio portfolio;
        portfolio::executeTransactionalPortfolioManagement(portfolio);
        return;
    }
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
        auto portfolio_manager_og = portfolio_manager;
        portfolio::executeMultiPortfolioManagement(portfolio_manager);
        if (portfolioInput.portfolio_src == "text") {
            generatePortfolioOverview(portfolio_manager,
                                      portfolioInput.gen_dir,
                                      portfolioInput.gen_dir + "../portfolio_overview.txt",
                                      portfolioInput.auto_save);
        }
        else if (portfolioInput.portfolio_src == "db") {
            valid = portfolio::updatePortfoliosDb(portfolio_manager_og,
                                                  portfolio_manager,
                                                  portfolioInput.db_dir,
                                                  portfolioInput.table_name,
                                                  portfolioInput.auto_save,
                                                  portfolioInput.low_latency_db_threshold,
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

static void executeStaticAppl(const std::string& mortgage_calculator_path_output,
                              const std::string& mortgage_calculator_path_input,
                              const InputDataMortgageCalculator& confInputMrtg) {

}

static void executeStaticAppl(const std::string& networth_projector_path_output,
                              const std::string& networth_projector_path_input,
                              const InputDataNetworthProjector& confInputNetw) {
    
}

std::array<bool, APPLICATION_IDX::LENGTH> resolveCfgAppSelector(const std::string& cfgFile) {
    std::array<bool, APPLICATION_IDX::LENGTH> retArr = {false};
    YmlCfg app_cfg(cfgFile);
    auto element = YmlCfg::createConfigElm("application-to-run");
    app_cfg.addConfigElement(element);
    if (app_cfg.readCfg(false, false)) {
        auto raw_str = element->value;
        std::vector<std::string> multi_string;
        splitStr(raw_str, ',', multi_string);
        for (auto& ms : multi_string) {
            trim(ms);
            convertStrToLowerCase(ms);
            if (ms == "portfolio_manager")
                retArr[APPLICATION_IDX::PORTFOLIO_MANAGER] = true;
            else if (ms == "networth_projector")
                retArr[APPLICATION_IDX::NETWORTH_PROJECTOR] = true;
            else if (ms == "mortgage_calculator")
                retArr[APPLICATION_IDX::MORTGAGE_CALCULATOR] = true;
        }
    }
    return retArr;
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    DirectoriesValues dirs;
    bool dirCfgRet = false;
    if (resolveCfg(dir_conf_file, dirs)) {
        dirCfgRet = true;
    }
    auto selected = resolveCfgAppSelector("conf/input/application_selector.yml");
    if (selected[APPLICATION_IDX::MORTGAGE_CALCULATOR]) {
        std::string mortgage_conf_file = "conf/input/mortgage.yml";
        MortgageValues mortg_cfg_values;
        InputDataMortgageCalculator input_data_mortg_from_yml;
        bool mortgCfgRet = false;
        if (resolveCfg(mortgage_conf_file, mortg_cfg_values)) {
            convertMortgageYmlData(input_data_mortg_from_yml, mortg_cfg_values);
            mortgCfgRet = true;
        }
        if (mortgCfgRet) {
            executeStaticAppl(dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
                              input_data_mortg_from_yml);
        }
    }
    if (selected[APPLICATION_IDX::NETWORTH_PROJECTOR]) {
        std::string networth_conf_file = "conf/input/networth.yml";
        NetworthValues nw_cfg_values;
        InputDataNetworthProjector input_data_nw_from_yml;
        bool netwCfgRet = false;
        if (resolveCfg(networth_conf_file, nw_cfg_values)) {
            convertNetworthYmlData(input_data_nw_from_yml, nw_cfg_values);
            netwCfgRet = true;
        }
        if (netwCfgRet) {
            executeStaticAppl(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
                              input_data_nw_from_yml);
        }
    }
    if (selected[APPLICATION_IDX::PORTFOLIO_MANAGER]) {
        executePortfolioMgr(dir_conf_file);
    }
    if (!selected[APPLICATION_IDX::PORTFOLIO_MANAGER]
        && !selected[APPLICATION_IDX::MORTGAGE_CALCULATOR]
        && !selected[APPLICATION_IDX::NETWORTH_PROJECTOR]) {
        std::string mortgage_conf_file = "conf/input/mortgage.yml";
        std::string networth_conf_file = "conf/input/networth.yml";
        MortgageValues mortg_cfg_values;
        NetworthValues nw_cfg_values;
        InputDataMortgageCalculator input_data_mortg_from_yml;
        InputDataNetworthProjector input_data_nw_from_yml;
        bool mortgCfgRet = false;
        bool netwCfgRet = false;
        if (resolveCfg(mortgage_conf_file, mortg_cfg_values)) {
            convertMortgageYmlData(input_data_mortg_from_yml, mortg_cfg_values);
            mortgCfgRet = true;
        }
        if (resolveCfg(networth_conf_file, nw_cfg_values)) {
            convertNetworthYmlData(input_data_nw_from_yml, nw_cfg_values);
            netwCfgRet = true;
        }
        if (doesUserWantStaticProgram() && netwCfgRet && mortgCfgRet) {
            executeStaticAppl(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
                              dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
                              input_data_nw_from_yml, input_data_mortg_from_yml);
        }
        if (getUserYesNo("portfolio manager mode")) {
            executePortfolioMgr(dir_conf_file);
        }
    }
    return 0;
}