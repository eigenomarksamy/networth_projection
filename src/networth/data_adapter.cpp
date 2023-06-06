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