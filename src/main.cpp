#include <iostream>
#include <string>
#include <sstream>
#include "utils.hpp"
#include "networth.hpp"
#include "mortgage.hpp"
#include "file_generator.hpp"
#include "data_adapter.hpp"
#include "conf_input_types.hpp"
#include "portfolio.hpp"
#include "utils.hpp"
#include "kafka.hpp"
#include "input_factory.hpp"
#include "yml_prsr.hpp"
#include "activation.hpp"

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
void fillDirectoryConfigurationNames(DirectoryGeneratorCfg& dircfg);
bool readDirectoryConfigurationValues(DirectoryGeneratorCfg& dir_cfg,
                                      const std::string conf_file);

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

static void executeCmdPromptUi(const DirectoryGeneratorCfg& directories) {
    InputDataContainer user_input;
    getProgramSelector(user_input);
    if (user_input.specifier == InputDataContainer::Specifier::NETWORTH_INPUT
        || user_input.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        executeStaticComputation(user_input, directories);
    }
    if (user_input.specifier == InputDataContainer::Specifier::PORTFOLIO_INPUT) {
        if (user_input.portfolio_manager.is_multi_prtfolio) {
            if (user_input.portfolio_manager.is_new) {
                portfolio::PortfolioManager portfolio_manager;
                executeMultiPortfolioManagement(portfolio_manager);
                generatePortfolioOverview(portfolio_manager,
                                          directories.portfolio_manager.nested.value,
                                          directories.portfolio_manager.overview.value);
            }
            else {
                portfolio::PortfolioManager portfolio_manager;
                if (getPortfolioFromFiles(portfolio_manager,
                                          user_input.portfolio_manager.load_all_portfolios,
                                          user_input.portfolio_manager.portfolio_list,
                                          directories.portfolio_manager.nested.value)) {
                    executeMultiPortfolioManagement(portfolio_manager);
                    generatePortfolioOverview(portfolio_manager,
                                          directories.portfolio_manager.nested.value,
                                          directories.portfolio_manager.overview.value);
                }
            }
        }
        else {
            if (user_input.portfolio_manager.is_new) {
                portfolio::Portfolio portfolio = portfolio::Portfolio(user_input.portfolio_manager.name);
                executePortfolioManagement(portfolio);
                generatePortfolioOverview(portfolio,
                                          directories.portfolio_manager.nested.value,
                                          directories.portfolio_manager.overview.value);
            }
            else {
                portfolio::Portfolio portfolio;
                if (getPortfolioFromFiles(portfolio,
                                          user_input.portfolio_manager.name,
                                          directories.portfolio_manager.nested.value)) {
                    executePortfolioManagement(portfolio);
                    generatePortfolioOverview(portfolio,
                                          directories.portfolio_manager.nested.value,
                                          directories.portfolio_manager.overview.value);
                }
            }
        }
    }
}

void fillDirectoryConfigurationNames(DirectoryGeneratorCfg& dircfg) {
    dircfg.mortgage_calculator.input.name = "generation.mortgage-calculator.input";
    dircfg.mortgage_calculator.output.name = "generation.mortgage-calculator.output";
    dircfg.networth_projector.input.name = "generation.networth-projector.input";
    dircfg.networth_projector.output.name = "generation.networth-projector.output";
    dircfg.portfolio_manager.nested.name = "generation.portfolio-manager.output.portfolios";
    dircfg.portfolio_manager.overview.name = "generation.portfolio-manager.output.overview";
}

bool readDirectoryConfigurationValues(DirectoryGeneratorCfg& dir_cfg,
                                      const std::string conf_file) {
    fillDirectoryConfigurationNames(dir_cfg);
    (void)getValueFromYml(conf_file, dir_cfg.networth_projector.input.name,
                          dir_cfg.networth_projector.input.value);
    (void)getValueFromYml(conf_file, dir_cfg.networth_projector.output.name,
                          dir_cfg.networth_projector.output.value);
    (void)getValueFromYml(conf_file, dir_cfg.mortgage_calculator.input.name,
                          dir_cfg.mortgage_calculator.input.value);
    (void)getValueFromYml(conf_file, dir_cfg.mortgage_calculator.output.name,
                          dir_cfg.mortgage_calculator.output.value);
    (void)getValueFromYml(conf_file, dir_cfg.portfolio_manager.nested.name,
                          dir_cfg.portfolio_manager.nested.value);
    (void)getValueFromYml(conf_file, dir_cfg.portfolio_manager.overview.name,
                          dir_cfg.portfolio_manager.overview.value);
    return true;
}

int main() {
    std::string dir_conf_file = "conf/directories.yml";
    DirectoryGeneratorCfg directories;
    (void)readDirectoryConfigurationValues(directories, dir_conf_file);
    executeCmdPromptUi(directories);
    return 0;
}