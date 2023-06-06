#ifndef DATA_ADAPTER_HPP_
#define DATA_ADAPTER_HPP_

#include <vector>
#include <string>

#include "net_worth.hpp"

class DataAdapter {

public:

    DataAdapter () { }

    ~DataAdapter() { }

    static std::vector<std::string> generateDataNames(NetWorth& netWorthObj);

    static std::vector<std::vector<std::string>> generateDataLines(NetWorth& netWorthObj);
};

#endif /* DATA_ADAPTER_HPP_ */