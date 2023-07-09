#include "mortgage.hpp"


std::vector<std::string> mortgage::MortgageAdapter::generateDataNames() const {
    return m_mortgageObj.getDataNames();
}

std::vector<std::vector<std::string>> mortgage::MortgageAdapter::generateDataLines() const {
    auto data = m_mortgageObj.getData();
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

std::vector<std::string> mortgage::MortgageAdapter::generateInputPreview() const {
    std::vector<std::string> lines;
    lines.push_back("Price: " + std::to_string(m_mortgageObj.getPrice()));
    lines.push_back("Number of months: " + std::to_string(m_mortgageObj.getNumMonths()));
    lines.push_back("Interest rate (%): " + std::to_string(m_mortgageObj.getInterestRate()));
    lines.push_back("Housing market inflation (%): " + std::to_string(m_mortgageObj.getMarketIncrease()));
    lines.push_back("Rent per month to compare: " + std::to_string(m_mortgageObj.getRentToCompare()));
    lines.push_back("Extra initial expenses (makelaar, overbidding, etc.): " + std::to_string(m_mortgageObj.getInitialFees()));
    lines.push_back("Rent annual increase (%): " + std::to_string(m_mortgageObj.getRentAnnualIncrease()));
    return lines;
}