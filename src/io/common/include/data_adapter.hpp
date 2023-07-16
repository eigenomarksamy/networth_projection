#ifndef DATA_ADAPTER_HPP_
#define DATA_ADAPTER_HPP_

#include <vector>
#include <string>
#include "portfolio.hpp"
#include "input_types_cfg.hpp"

class DataAdapter {

public:

    DataAdapter () { }

    ~DataAdapter() { }

    static std::vector<std::string> generatePortfolioLines(const portfolio::Portfolio& portfolioObj);

    static std::vector<std::string> generatePortfolioLines(const portfolio::PortfolioManager& portfolioMgrObj);
};

#endif /* DATA_ADAPTER_HPP_ */