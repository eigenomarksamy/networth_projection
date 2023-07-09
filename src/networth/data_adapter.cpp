#include "networth.hpp"


std::vector<std::string> networth::NetworthAdapter::generateDataNames() const {
    return m_networthObj.getDataNames();
}

std::vector<std::vector<std::string>> networth::NetworthAdapter::generateDataLines() const {
    auto data = m_networthObj.getData();
    std::vector<std::vector<std::string>> retDataLines;
    std::vector<std::string> tmp_row;
    for (const auto& d : data) {
        for (const auto& a : d) {
            tmp_row.push_back(std::to_string(a));
        }
        retDataLines.push_back(tmp_row);
        tmp_row.clear();
    }
    return retDataLines;
}

std::vector<std::string> networth::NetworthAdapter::generateInputPreview() const {
    std::vector<std::string> lines;
    lines.push_back("Current Age: " + std::to_string(m_networthObj.getInitialAge()));
    lines.push_back("Initial Net worth: " + std::to_string(m_networthObj.getInitialNetworth()));
    lines.push_back("Yearly income: " + std::to_string(m_networthObj.getYearlyIncome()));
    lines.push_back("Yearly increase in Salary: " + std::to_string(m_networthObj.getYearlyIncomeIncrease()));
    lines.push_back("Portfolio yearly return: " + std::to_string(m_networthObj.getPortfolioYearlyReturn()));
    lines.push_back("Portfolio fees: " + std::to_string(m_networthObj.getPortfolioFees()));
    lines.push_back("Percentage Investing: " + std::to_string(m_networthObj.getPercentageOfInvesting()));
    lines.push_back("Age of retirement: " + std::to_string(m_networthObj.getAgeRetirement()));
    return lines;
}