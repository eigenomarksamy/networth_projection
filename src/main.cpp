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


networth::NetWorth computeNetworthData(const InputDataNetworthProjector& userInput) {
    networth::NetWorth networth(userInput.init_nw, userInput.year_income,
                                userInput.age_retirement, userInput.current_age,
                                networth::NetWorth::Percentages({
                                             userInput.year_increase,
                                             userInput.port_yearly_ret,
                                             userInput.port_fees,
                                             userInput.inv_yearly}));
    std::unordered_map<uint32_t, float_t> extra_money_map;
    convert2DVectorToUnorderedMap(userInput.year_to_amount,
                                  extra_money_map);
    networth.setDepWithdrawalPlan(extra_money_map);
    networth.computeData();
    networth.printTabulatedData();
    return networth;
}

mortgage::Mortgage computeMortgageData(const InputDataMortgageCalculator& userInput) {
    mortgage::Mortgage mortgage(userInput.price, userInput.num_months,
                      userInput.interest_rate, userInput.market_increase,
                      userInput.rent_to_compare, userInput.makelaar_fees,
                      userInput.rent_annu_increase);
    mortgage.computeData();
    mortgage.printTabulatedData();
    return mortgage;
}

void generateDataCsv(const networth::NetWorth netWorth, std::string fileName) {
    auto headers = DataAdapter::generateDataNames(netWorth);
    auto lines = DataAdapter::generateDataLines(netWorth);
    FileGenerator file_h(fileName);
    file_h.generateCsv(headers, lines);
}

void generateDataCsv(const mortgage::Mortgage mortgage, std::string fileName) {
    auto headers = DataAdapter::generateDataNames(mortgage);
    auto lines = DataAdapter::generateDataLines(mortgage);
    FileGenerator file_h(fileName);
    file_h.generateCsv(headers, lines);
}

void generateInputTxt(const InputDataContainer& input, std::string fileName) {
    auto lines = DataAdapter::generateInputLines(input);
    FileGenerator file_h(fileName);
    file_h.generateTxt(lines);
}

void generateFiles(const networth::NetWorth& net_worth,
                   const InputDataContainer& user_input,
                   const std::string& outputFile,
                   const std::string& inputFile) {
    generateDataCsv(net_worth, outputFile);
    generateInputTxt(user_input, inputFile);
}

void generateFiles(const mortgage::Mortgage& mortgage,
                   const InputDataContainer& user_input,
                   const std::string& outputFile,
                   const std::string& inputFile) {
    generateDataCsv(mortgage, outputFile);
    generateInputTxt(user_input, inputFile);
}

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

static void executeCmdPromptUi(const std::string& networth_out,
                               const std::string& networth_in,
                               const std::string& mortgage_out,
                               const std::string& mortgage_in,
                               const std::string& portfolio_directory,
                               const std::string& portfolio_overview) {
    InputDataContainer user_input;
    getProgramSelector(user_input);
    if (user_input.specifier == InputDataContainer::Specifier::NETWORTH_INPUT) {
        auto net_worth = computeNetworthData(user_input.networth_projector);
        generateFiles(net_worth, user_input, networth_out, networth_in);
    }
    else if (user_input.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        auto mortgage = computeMortgageData(user_input.mortgage_calculator);
        generateFiles(mortgage, user_input, mortgage_out, mortgage_in);
    }
    else if (user_input.specifier == InputDataContainer::Specifier::PORTFOLIO_INPUT) {
        if (user_input.portfolio_manager.is_multi_prtfolio) {
            if (user_input.portfolio_manager.is_new) {
                portfolio::PortfolioManager portfolio_manager;
                executeMultiPortfolioManagement(portfolio_manager);
                generatePortfolioOverview(portfolio_manager,
                                          portfolio_directory,
                                          portfolio_overview);
            }
            else {
                portfolio::PortfolioManager portfolio_manager;
                if (getPortfolioFromFiles(portfolio_manager,
                                          user_input.portfolio_manager.load_all_portfolios,
                                          user_input.portfolio_manager.portfolio_list,
                                          portfolio_directory)) {
                    executeMultiPortfolioManagement(portfolio_manager);
                    generatePortfolioOverview(portfolio_manager,
                                              portfolio_directory,
                                              portfolio_overview);
                }
            }
        }
        else {
            if (user_input.portfolio_manager.is_new) {
                portfolio::Portfolio portfolio = portfolio::Portfolio(user_input.portfolio_manager.name);
                executePortfolioManagement(portfolio);
                generatePortfolioOverview(portfolio,
                                          portfolio_directory,
                                          portfolio_overview);
            }
            else {
                portfolio::Portfolio portfolio;
                if (getPortfolioFromFiles(portfolio, user_input.portfolio_manager.name, portfolio_directory)) {
                    executePortfolioManagement(portfolio);
                    generatePortfolioOverview(portfolio,
                                              portfolio_directory,
                                              portfolio_overview);
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
    executeCmdPromptUi(directories.networth_projector.output.value,
                       directories.networth_projector.input.value,
                       directories.mortgage_calculator.output.value,
                       directories.mortgage_calculator.input.value,
                       directories.portfolio_manager.nested.value,
                       directories.portfolio_manager.overview.value);
    return 0;
}