#ifndef STR_ADAPTER_HPP_
#define STR_ADAPTER_HPP_

#include <vector>
#include <string>
#include "computation_adapter.hpp"

class StrStreamGenerator {

public:

    virtual std::vector<std::string> generateDataNames() const = 0;
    virtual std::vector<std::vector<std::string>> generateDataLines() const = 0;
    virtual std::vector<std::string> generateInputPreview() const = 0;

    struct NamesWithLines {
        std::vector<std::string> names;
        std::vector<std::vector<std::string>> lines;
    };

    NamesWithLines generateNamesWithLines() const;
};

#endif /* STR_ADAPTER_HPP_ */