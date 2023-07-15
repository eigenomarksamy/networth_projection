#include <iostream>
#include <string>
#include <sstream>
#include "utils.hpp"
#include "networth.hpp"
#include "mortgage.hpp"
#include "file_generator.hpp"
#include "data_adapter.hpp"
#include "portfolio.hpp"
#include "utils.hpp"
#include "kafka.hpp"
#include "input_factory.hpp"
#include "yml_prsr.hpp"
#include "activation.hpp"
#include "conf_resolver.hpp"
#include "appl_conf_types.hpp"

void generatePortfolioFiles(const portfolio::PortfolioManager& portfolioMgr,
                            const std::string& directory);
void generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                            const std::string& directory);
void generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                               const std::string& directory,
                               const std::string& outputFile);
void generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                               const std::string& directory,
                               const std::string& outputFile);
bool getPortfolioFromFiles(portfolio::Portfolio& portfolio,
                           const std::string& name,
                           const std::string& directory);
bool getPortfolioFromFiles(portfolio::PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios,
                           const std::string& directory);
static void executePortfolioMgr(const InputPortfolioManager& portfolioInput,
                                const std::string& porto_mgr_path_nested,
                                const std::string& porto_mgr_path_overview);
static void executeCmdPromptUi(const std::string& networth_projector_path_output,
                               const std::string& networth_projector_path_input,
                               const std::string& mortgage_caculator_path_output,
                               const std::string& mortgage_calculator_path_input,
                               const std::string& porto_mgr_path_nested,
                               const std::string& porto_mgr_path_overview);
static void resolveCfg(const std::string& confPath, DirectoriesValues& dirs);
static void resolveCfg(const std::string& confPath, NetworthValues& values);
static void resolveCfg(const std::string& confPath, MortgageValues& values);

void generatePortfolioFiles(const portfolio::PortfolioManager& portfolioMgr,
                            const std::string& directory) {
    for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
        auto portfolio = portfolioMgr.getPortfolio(i);
        savePortfolio(portfolio, directory + portfolio.getName());
    }
}

void generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                            const std::string& directory) {
    savePortfolio(portfolio, directory + portfolio.getName());
}

void generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                               const std::string& directory,
                               const std::string& outputFile) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolio);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolio, directory);
}

void generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                               const std::string& directory,
                               const std::string& outputFile) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolioMgr);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolioMgr, directory);
}

bool getPortfolioFromFiles(portfolio::Portfolio& portfolio,
                           const std::string& name,
                           const std::string& directory) {
    return loadPortfolio(portfolio, directory + name);
}

bool getPortfolioFromFiles(portfolio::PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios,
                           const std::string& directory) {
    bool status = true;
    std::string directoryPath = directory;
    std::vector<std::string> names;
    if (load_all_portfolios)
        names = getFileNames(directoryPath);
    else
        names = list_portfolios;
    for (const auto& name : names) {
        portfolio::Portfolio portfolio;
        if (getPortfolioFromFiles(portfolio, name, directory)) {
            portfolioMgr.addPortfolio(portfolio);
            status &= true;
        }
        else {
            status = false;
        }
    }
    return status;
}

static void executePortfolioMgr(const InputPortfolioManager& portfolioInput,
                                const std::string& porto_mgr_path_nested,
                                const std::string& porto_mgr_path_overview) {
    if (portfolioInput.is_multi_prtfolio) {
        if (portfolioInput.is_new) {
            portfolio::PortfolioManager portfolio_manager;
            executeMultiPortfolioManagement(portfolio_manager);
            generatePortfolioOverview(portfolio_manager,
                                      porto_mgr_path_nested,
                                      porto_mgr_path_overview);
        }
        else {
            portfolio::PortfolioManager portfolio_manager;
            if (getPortfolioFromFiles(portfolio_manager,
                                      portfolioInput.load_all_portfolios,
                                      portfolioInput.portfolio_list,
                                      porto_mgr_path_nested)) {
                executeMultiPortfolioManagement(portfolio_manager);
                generatePortfolioOverview(portfolio_manager,
                                          porto_mgr_path_nested,
                                          porto_mgr_path_overview);
            }
        }
    }
    else {
        if (portfolioInput.is_new) {
            portfolio::Portfolio portfolio = portfolio::Portfolio(portfolioInput.name);
            executePortfolioManagement(portfolio);
            generatePortfolioOverview(portfolio,
                                      porto_mgr_path_nested,
                                      porto_mgr_path_overview);
        }
        else {
            portfolio::Portfolio portfolio;
            if (getPortfolioFromFiles(portfolio,
                                      portfolioInput.name,
                                      porto_mgr_path_nested)) {
                executePortfolioManagement(portfolio);
                generatePortfolioOverview(portfolio,
                                          porto_mgr_path_nested,
                                          porto_mgr_path_overview);
            }
        }
    }
}

static void executeCmdPromptUi(const std::string& networth_projector_path_output,
                               const std::string& networth_projector_path_input,
                               const std::string& mortgage_caculator_path_output,
                               const std::string& mortgage_calculator_path_input,
                               const std::string& porto_mgr_path_nested,
                               const std::string& porto_mgr_path_overview) {
    InputDataContainer user_input;
    getProgramSelector(user_input);
    if (user_input.specifier == InputDataContainer::Specifier::NETWORTH_INPUT
        || user_input.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        executeStaticComputation(user_input,
                                 networth_projector_path_output,
                                 networth_projector_path_input,
                                 mortgage_caculator_path_output,
                                 mortgage_calculator_path_input);
    }
    else if (user_input.specifier == InputDataContainer::Specifier::PORTFOLIO_INPUT) {
        executePortfolioMgr(user_input.portfolio_manager,
                            porto_mgr_path_nested,
                            porto_mgr_path_overview);
    }
    else {
        return;
    }
}

static void resolveCfg(const std::string& confPath, DirectoriesValues& dirs) {
    YmlCfg dir_cfg(confPath);
    dirs.mortg_calc_in = YmlCfg::createConfigElm("generation.mortgage-calculator.input");
    dir_cfg.addConfigElement(dirs.mortg_calc_in);
    dirs.netwo_calc_in = YmlCfg::createConfigElm("generation.networth-projector.input");
    dir_cfg.addConfigElement(dirs.netwo_calc_in);
    dirs.mortg_calc_out = YmlCfg::createConfigElm("generation.mortgage-calculator.output");
    dir_cfg.addConfigElement(dirs.mortg_calc_out);
    dirs.netwo_calc_out = YmlCfg::createConfigElm("generation.networth-projector.output");
    dir_cfg.addConfigElement(dirs.netwo_calc_out);
    dirs.porto_dirs_out = YmlCfg::createConfigElm("generation.portfolio-manager.output.portfolios");
    dir_cfg.addConfigElement(dirs.porto_dirs_out);
    dirs.porto_overview = YmlCfg::createConfigElm("generation.portfolio-manager.output.overview");
    dir_cfg.addConfigElement(dirs.porto_overview);
    dir_cfg.readCfg(false, false);
}

static void resolveCfg(const std::string& confPath, NetworthValues& values) {
    YmlCfg cfg(confPath);
    values.age_current = YmlCfg::createConfigElm("age.current");
    cfg.addConfigElement(values.age_current);
    values.age_retirement = YmlCfg::createConfigElm("age.retirement");
    cfg.addConfigElement(values.age_retirement);
    values.initial_networth = YmlCfg::createConfigElm("initial-networth");
    cfg.addConfigElement(values.initial_networth);
    values.portfolio_fees = YmlCfg::createConfigElm("yearly.percentage.portfolio.fees");
    cfg.addConfigElement(values.portfolio_fees);
    values.yearly_income = YmlCfg::createConfigElm("yearly.income");
    cfg.addConfigElement(values.yearly_income);
    values.yearly_income_raise = YmlCfg::createConfigElm("yearly.percentage.income-raise");
    cfg.addConfigElement(values.yearly_income_raise);
    values.yearly_investment = YmlCfg::createConfigElm("yearly.percentage.investment");
    cfg.addConfigElement(values.yearly_investment);
    values.yearly_roi = YmlCfg::createConfigElm("yearly.percentage.portfolio.roi");
    cfg.addConfigElement(values.yearly_roi);
    values.deposit_withdrawal = YmlCfg::createConfigElm("deposit-withdrawal", confPath);
    cfg.addConfigElement(values.deposit_withdrawal);
    cfg.readCfg(true, false);
}

static void resolveCfg(const std::string& confPath, MortgageValues& values) {
    YmlCfg cfg(confPath);
    values.annual_rent_increase = YmlCfg::createConfigElm("rent-to-compare.annual-increase");
    cfg.addConfigElement(values.annual_rent_increase);
    values.initial_overhead = YmlCfg::createConfigElm("price.overhead");
    cfg.addConfigElement(values.initial_overhead);
    values.interest_rate = YmlCfg::createConfigElm("interest-rate");
    cfg.addConfigElement(values.interest_rate);
    values.market_inflation = YmlCfg::createConfigElm("market-inflation");
    cfg.addConfigElement(values.market_inflation);
    values.original_price = YmlCfg::createConfigElm("price.unit");
    cfg.addConfigElement(values.original_price);
    values.rent_to_compare = YmlCfg::createConfigElm("rent-to-compare.monthly");
    cfg.addConfigElement(values.rent_to_compare);
    values.time_to_settle_months = YmlCfg::createConfigElm("time-to-settle.months");
    cfg.addConfigElement(values.time_to_settle_months);
    values.time_to_settle_years = YmlCfg::createConfigElm("time-to-settle.years");
    cfg.addConfigElement(values.time_to_settle_years);
    cfg.readCfg(true, false);
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    std::string networth_conf_file = "conf/input/networth.yml";
    std::string mortgage_conf_file = "conf/input/mortgage.yml";
    DirectoriesValues dirs;
    NetworthValues nw_cfg_values;
    MortgageValues mortg_cfg_values;
    resolveCfg(dir_conf_file, dirs);
    resolveCfg(networth_conf_file, nw_cfg_values);
    resolveCfg(mortgage_conf_file, mortg_cfg_values);
    executeCmdPromptUi(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
                       dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
                       dirs.porto_dirs_out->value, dirs.porto_overview->value);
    return 0;
}