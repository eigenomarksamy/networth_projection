#include "data_adapter.hpp"

std::vector<std::string> DataAdapter::generateDataNames(NetWorth& netWorthObj) {
    return netWorthObj.getDataNames();
}

std::vector<std::vector<std::string>> DataAdapter::generateDataLines(NetWorth& netWorthObj) {
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

std::vector<std::string> DataAdapter::generateInputLines(Input& inputObj) {
    std::vector<std::string> lines;
    lines.push_back("Current Age: " + std::to_string(inputObj.current_age));
    lines.push_back("Initial Net worth: " + std::to_string(inputObj.init_nw));
    lines.push_back("Yearly income: " + std::to_string(inputObj.year_income));
    lines.push_back("Yearly increase in Salary: " + std::to_string(inputObj.year_increase));
    lines.push_back("Portfolio yearly return: " + std::to_string(inputObj.port_yearly_ret));
    lines.push_back("Portfolio fees: " + std::to_string(inputObj.port_fees));
    lines.push_back("Percentage Investing: " + std::to_string(inputObj.perc_inv));
    lines.push_back("Age of retirement: " + std::to_string(inputObj.age_retirement));
    return lines;
}