#include "data_adapter.hpp"

std::vector<std::string> DataAdapter::generateDataNames(const NetWorth& netWorthObj) {
    return netWorthObj.getDataNames();
}

std::vector<std::string> DataAdapter::generateDataNames(const Mortgage& mortgageObj) {
    return mortgageObj.getDataNames();
}

std::vector<std::vector<std::string>> DataAdapter::generateDataLines(const NetWorth& netWorthObj) {
    auto nw_data = netWorthObj.getData();
    std::vector<std::vector<std::string>> retDataLines;
    std::vector<std::string> tmp_row;
    for (const auto& d : nw_data) {
        for (const auto& a : d) {
            tmp_row.push_back(std::to_string(a));
        }
        retDataLines.push_back(tmp_row);
        tmp_row.clear();
    }
    return retDataLines;
}

std::vector<std::vector<std::string>> DataAdapter::generateDataLines(const Mortgage& mortgaeObj) {
    auto mort_data = mortgaeObj.getData();
    std::vector<std::vector<std::string>> retDataLines;
    std::vector<std::string> tmp_row;
    for (const auto& d : mort_data) {
        for (const auto& a : d) {
            tmp_row.push_back(std::to_string(a));
        }
        retDataLines.push_back(tmp_row);
        tmp_row.clear();
    }
    return retDataLines;
}

void DataAdapter::generateNetworthProjectorInputLines(const InputDataNetworthProjector& inputObj, std::vector<std::string>& lines) {
    lines.push_back("Current Age: " + std::to_string(inputObj.current_age));
    lines.push_back("Initial Net worth: " + std::to_string(inputObj.init_nw));
    lines.push_back("Yearly income: " + std::to_string(inputObj.year_income));
    lines.push_back("Yearly increase in Salary: " + std::to_string(inputObj.year_increase));
    lines.push_back("Portfolio yearly return: " + std::to_string(inputObj.port_yearly_ret));
    lines.push_back("Portfolio fees: " + std::to_string(inputObj.port_fees));
    lines.push_back("Percentage Investing: " + std::to_string(inputObj.inv_yearly));
    lines.push_back("Age of retirement: " + std::to_string(inputObj.age_retirement));
}

void DataAdapter::generateMortgageCalculatorInputLines(const InputDataMortgageCalculator& inputObj, std::vector<std::string>& lines) {
    lines.push_back("Price: " + std::to_string(inputObj.price));
    lines.push_back("Number of months: " + std::to_string(inputObj.num_months));
    lines.push_back("Interest rate: " + std::to_string(inputObj.interest_rate));
}

std::vector<std::string> DataAdapter::generateInputLines(const InputDataContainer& inputObj) {
    std::vector<std::string> lines;
    if (inputObj.specifier == InputDataContainer::Specifier::NETWORTH_INPUT) {
        generateNetworthProjectorInputLines(inputObj.networth_projector, lines);
    }
    else if (inputObj.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        generateMortgageCalculatorInputLines(inputObj.mortgage_calculator, lines);
    }
    return lines;
}