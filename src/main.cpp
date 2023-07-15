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
static void resolveDirCfg(const std::string& confPath, DirectoriesValues& dirs);

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

static void resolveDirCfg(const std::string& confPath, DirectoriesValues& dirs) {
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

void resolveNetworthCfg(const std::string& confPath, NetworthValues& values) {
    YmlCfg cfg(confPath);
    values.age_current = YmlCfg::createConfigElm("current-age");
    cfg.addConfigElement(values.age_current);
    values.age_retirement = YmlCfg::createConfigElm("age-retirement");
    cfg.addConfigElement(values.age_retirement);
    values.initial_networth = YmlCfg::createConfigElm("initial-networth");
    cfg.addConfigElement(values.initial_networth);
    values.portfolio_fees = YmlCfg::createConfigElm("portfolio-fees");
    cfg.addConfigElement(values.portfolio_fees);
    values.yearly_income = YmlCfg::createConfigElm("yearly-income");
    cfg.addConfigElement(values.yearly_income);
    values.yearly_income_raise = YmlCfg::createConfigElm("yearly-income-raise");
    cfg.addConfigElement(values.yearly_income_raise);
    values.yearly_investment = YmlCfg::createConfigElm("yearly-investing");
    cfg.addConfigElement(values.yearly_investment);
    values.yearly_roi = YmlCfg::createConfigElm("return-estimate");
    cfg.addConfigElement(values.yearly_roi);
    values.deposit_withdrawal = YmlCfg::createConfigElm("deposit-withdrawal", confPath);
    cfg.addConfigElement(values.deposit_withdrawal);
    cfg.readCfg(false, false);
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    std::string networth_conf_file = "conf/input/networth.yml";
    DirectoriesValues dirs;
    NetworthValues nw_cfg_values;
    resolveDirCfg(dir_conf_file, dirs);
    resolveNetworthCfg(networth_conf_file, nw_cfg_values);
    // executeCmdPromptUi(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
    //                    dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
    //                    dirs.porto_dirs_out->value, dirs.porto_overview->value);
    return 0;
}