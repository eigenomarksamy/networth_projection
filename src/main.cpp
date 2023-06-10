#include <iostream>
#include <string>
#include "net_worth.hpp"
#include "file_manager.hpp"
#include "data_adapter.hpp"
#include "input_handler.hpp"

NetWorth computeNetworthData(const InputDataNetworthProjector& userInput) {
    NetWorth networth(userInput.init_nw, userInput.year_income,
                      userInput.age_retirement, userInput.current_age,
                      NetWorth::Percentages({userInput.year_increase,
                                             userInput.port_yearly_ret,
                                             userInput.port_fees,
                                             userInput.inv_yearly}));
    networth.computeData();
    networth.printTabulatedData();
    return networth;
}

void generateDataCsv(const NetWorth netWorth, std::string fileName) {
    auto headers = DataAdapter::generateDataNames(netWorth);
    auto lines = DataAdapter::generateDataLines(netWorth);
    FileHandler file_h(fileName);
    file_h.generateCsv(headers, lines);
}

void generateInputTxt(const InputDataContainer& input, std::string fileName) {
    auto lines = DataAdapter::generateInputLines(input);
    FileHandler file_h(fileName);
    file_h.generateTxt(lines);
}

void generateFiles(const NetWorth& net_worth, const InputDataContainer& user_input) {
    generateDataCsv(net_worth, "gen\\nw_data_out.csv");
    generateInputTxt(user_input, "gen\\nw_input.txt");
}

int main() {
    InputDataContainer user_input;
    std::cout << "App: Launched!\n";
    getUserSelection(user_input);
    auto net_worth = computeNetworthData(user_input.networth_projector);
    generateFiles(net_worth, user_input);
    return 0;
}