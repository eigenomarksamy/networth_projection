#include <iostream>
#include <string>
#include <sstream>
#include "utils.hpp"
#include "networth.hpp"
#include "mortgage.hpp"
#include "file_manager.hpp"
#include "data_adapter.hpp"
#include "input_handler.hpp"
#include "portfolio.hpp"
#include "utils.hpp"
#include "continuous_input.hpp"
#include "kafka_wrapper.hpp"


NetWorth computeNetworthData(const InputDataNetworthProjector& userInput) {
    NetWorth networth(userInput.init_nw, userInput.year_income,
                      userInput.age_retirement, userInput.current_age,
                      NetWorth::Percentages({userInput.year_increase,
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

Mortgage computeMortgageData(const InputDataMortgageCalculator& userInput) {
    Mortgage mortgage(userInput.price, userInput.num_months,
                      userInput.interest_rate, userInput.market_increase,
                      userInput.rent_to_compare, userInput.makelaar_fees,
                      userInput.rent_annu_increase);
    mortgage.computeData();
    mortgage.printTabulatedData();
    return mortgage;
}

void generateDataCsv(const NetWorth netWorth, std::string fileName) {
    auto headers = DataAdapter::generateDataNames(netWorth);
    auto lines = DataAdapter::generateDataLines(netWorth);
    FileGenerator file_h(fileName);
    file_h.generateCsv(headers, lines);
}

void generateDataCsv(const Mortgage mortgage, std::string fileName) {
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

void generateFiles(const NetWorth& net_worth, const InputDataContainer& user_input) {
    generateDataCsv(net_worth, "gen/nw_data_out.csv");
    generateInputTxt(user_input, "gen/nw_input.txt");
}

void generateFiles(const Mortgage& mortgage, const InputDataContainer& user_input) {
    generateDataCsv(mortgage, "gen/mortgage_data_out.csv");
    generateInputTxt(user_input, "gen/mortgage_input.txt");
}

void generatePortfolioFiles(const PortfolioManager& portfolioMgr) {
    for (auto i = 0; i < portfolioMgr.getNumPortfolios(); ++i) {
        auto portfolio = portfolioMgr.getPortfolio(i);
        savePortfolio(portfolio, "gen/portfolios/" + portfolio.getName());
    }
}

void generatePortfolioFiles(const Portfolio& portfolio) {
    savePortfolio(portfolio, "gen/portfolios/" + portfolio.getName());
}

void generatePortfolioOverview(const Portfolio& portfolio) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolio);
    FileGenerator file("gen/portfolios_overview.txt");
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolio);
}

void generatePortfolioOverview(const PortfolioManager& portfolioMgr) {
    auto portfolioTxt = DataAdapter::generatePortfolioLines(portfolioMgr);
    FileGenerator file("gen/portfolios_overview.txt");
    file.generateTxt(portfolioTxt);
    generatePortfolioFiles(portfolioMgr);
}

bool getPortfolioFromFiles(Portfolio& portfolio, const std::string& name) {
    return loadPortfolio(portfolio, "gen/portfolios/" + name);
}

bool getPortfolioFromFiles(PortfolioManager& portfolioMgr,
                           const bool load_all_portfolios,
                           const std::vector<std::string>& list_portfolios) {
    bool status = true;
    std::string directoryPath = "gen/portfolios";
    std::vector<std::string> names;
    if (load_all_portfolios)
        names = getFileNames(directoryPath);
    else
        names = list_portfolios;
    for (const auto& name : names) {
        Portfolio portfolio;
        if (getPortfolioFromFiles(portfolio, name)) {
            portfolioMgr.addPortfolio(portfolio);
            status &= true;
        }
        else {
            status = false;
        }
    }
    return status;
}

void testKafka() {
    // KafkaProducer producer("localhost:9092");
    // if (producer.initialize()) {
    //     producer.produce("TopicA", "Message 1 for Topic A");
    //     producer.produce("TopicA", "Message 2 for Topic A");
    //     producer.produce("TopicB", "Message 1 for Topic B");
    //     producer.produce("TopicB", "Message 2 for Topic B");
    // }
    std::string brokerList = "localhost:9092";
    std::string topic = "your_topic";
    std::string message = "Hello, Kafka!";

    KafkaWrapper kafkaWrapper(brokerList);
    kafkaWrapper.produce(topic, message);
}

int main() {
    InputDataContainer user_input;
    getUserSelection(user_input);
    if (user_input.specifier == InputDataContainer::Specifier::NETWORTH_INPUT) {
        auto net_worth = computeNetworthData(user_input.networth_projector);
        generateFiles(net_worth, user_input);
    }
    else if (user_input.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        auto mortgage = computeMortgageData(user_input.mortgage_calculator);
        generateFiles(mortgage, user_input);
    }
    else if (user_input.specifier == InputDataContainer::Specifier::PORTFOLIO_INPUT) {
        if (user_input.portfolio_manager.is_multi_prtfolio) {
            if (user_input.portfolio_manager.is_new) {
                PortfolioManager portfolio_manager;
                executeMultiPortfolioManagement(portfolio_manager);
                generatePortfolioOverview(portfolio_manager);
            }
            else {
                PortfolioManager portfolio_manager;
                if (getPortfolioFromFiles(portfolio_manager,
                                          user_input.portfolio_manager.load_all_portfolios,
                                          user_input.portfolio_manager.portfolio_list)) {
                    executeMultiPortfolioManagement(portfolio_manager);
                    generatePortfolioOverview(portfolio_manager);
                }
            }
        }
        else {
            if (user_input.portfolio_manager.is_new) {
                Portfolio portfolio = Portfolio(user_input.portfolio_manager.name);
                executePortfolioManagement(portfolio);
                generatePortfolioOverview(portfolio);
            }
            else {
                Portfolio portfolio;
                if (getPortfolioFromFiles(portfolio, user_input.portfolio_manager.name)) {
                    executePortfolioManagement(portfolio);
                    generatePortfolioOverview(portfolio);
                }
            }
        }
    }
    return 0;
}