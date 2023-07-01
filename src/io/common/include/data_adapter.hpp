#ifndef DATA_ADAPTER_HPP_
#define DATA_ADAPTER_HPP_

#include <vector>
#include <string>

#include "networth.hpp"
#include "mortgage.hpp"
#include "portfolio.hpp"
#include "input_types_cfg.hpp"

class DataAdapter {

private:

    static void generateNetworthProjectorInputLines(const InputDataNetworthProjector& inputObj, std::vector<std::string>& lines);
    static void generateMortgageCalculatorInputLines(const InputDataMortgageCalculator& inputObj, std::vector<std::string>& lines);

public:

    DataAdapter () { }

    ~DataAdapter() { }

    static std::vector<std::string> generateDataNames(const networth::NetWorth& netWorthObj);

    static std::vector<std::string> generateDataNames(const mortgage::Mortgage& mortgageObj);

    static std::vector<std::vector<std::string>> generateDataLines(const networth::NetWorth& netWorthObj);

    static std::vector<std::vector<std::string>> generateDataLines(const mortgage::Mortgage& mortgageObj);

    static std::vector<std::string> generateInputLines(const InputDataContainer& inputObj);

    static std::vector<std::string> generatePortfolioLines(const Portfolio& portfolioObj);

    static std::vector<std::string> generatePortfolioLines(const PortfolioManager& portfolioMgrObj);
};

#endif /* DATA_ADAPTER_HPP_ */