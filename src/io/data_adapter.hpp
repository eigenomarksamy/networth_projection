#ifndef DATA_ADAPTER_HPP_
#define DATA_ADAPTER_HPP_

#include <vector>
#include <string>

#include "net_worth.hpp"
#include "input_handler.hpp"

class DataAdapter {

public:

    DataAdapter () { }

    ~DataAdapter() { }

    static std::vector<std::string> generateDataNames(NetWorth& netWorthObj);

    static std::vector<std::vector<std::string>> generateDataLines(NetWorth& netWorthObj);

    static std::vector<std::string> generateInputLines(InputNwProjection& inputObj);
};

#endif /* DATA_ADAPTER_HPP_ */