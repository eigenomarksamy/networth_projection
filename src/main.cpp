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
                            const std::string& directory,
                            const bool autoSave);
void generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                            const std::string& directory,
                            const bool autoSave);
void generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave);
void generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave);
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
                               const InputDataNetworthProjector& confInputNetw,
                               const InputDataMortgageCalculator& confInputMrtg);

void generatePortfolioFiles(const portfolio::PortfolioManager& portfolioMgr,
                            const std::string& directory,
                            const bool autoSave) {
    for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
        auto portfolio = portfolioMgr.getPortfolio(i);
        std::string question = "save portfolio " + portfolio.getName();
        if (autoSave || getUserYesNo(question)) {
            savePortfolio(portfolio, directory + portfolio.getName());
        }
    }
}

void generatePortfolioFiles(const portfolio::Portfolio& portfolio,
                            const std::string& directory,
                            const bool autoSave) {
    if (autoSave || getUserYesNo("save portfolio " + portfolio.getName())) {
        savePortfolio(portfolio, directory + portfolio.getName());
    }
}

void generatePortfolioOverview(const portfolio::Portfolio& portfolio,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolio);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolio, directory, autoSave);
}

void generatePortfolioOverview(const portfolio::PortfolioManager& portfolioMgr,
                               const std::string& directory,
                               const std::string& outputFile,
                               const bool autoSave) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolioMgr);
    FileGenerator file(outputFile);
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolioMgr, directory, autoSave);
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
        portfolio::PortfolioManager portfolio_manager;
        bool valid = false;
        if (portfolioInput.is_new || getPortfolioFromFiles(portfolio_manager,
                                    portfolioInput.load_all_portfolios,
                                    portfolioInput.portfolio_list,
                                    porto_mgr_path_nested)) {
            valid = true;
        }
        if (valid) {
            executeMultiPortfolioManagement(portfolio_manager);
            generatePortfolioOverview(portfolio_manager,
                                    porto_mgr_path_nested,
                                    porto_mgr_path_overview,
                                    portfolioInput.auto_save);
        }
    }
    else {
        portfolio::Portfolio portfolio = portfolio::Portfolio(portfolioInput.name);
        bool valid = false;
        if (portfolioInput.is_new || getPortfolioFromFiles(portfolio,
                                      portfolioInput.name,
                                      porto_mgr_path_nested)) {
            valid = true;
        }
        if (valid) {
            executePortfolioManagement(portfolio);
            generatePortfolioOverview(portfolio,
                                        porto_mgr_path_nested,
                                        porto_mgr_path_overview,
                                        portfolioInput.auto_save);
        }
    }
}

static void executeCmdPromptUi(const std::string& networth_projector_path_output,
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
    // else if (user_input.specifier == InputDataContainer::Specifier::PORTFOLIO_INPUT) {
    //     executePortfolioMgr(user_input.portfolio_manager,
    //                         porto_mgr_path_nested,
    //                         porto_mgr_path_overview);
    // }
    else {
        return;
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
    executeCmdPromptUi(dirs.netwo_calc_out->value, dirs.netwo_calc_in->value,
                       dirs.mortg_calc_out->value, dirs.mortg_calc_in->value,
                       input_data_nw_from_yml, input_data_mortg_from_yml);
    return 0;
}