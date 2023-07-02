#ifndef CONF_INPUT_TYPES_HPP_
#define CONF_INPUT_TYPES_HPP_

#include <string>

struct DirectoryCommonCfg {
    std::string name;
    std::string value;
};

struct DirectoryStaticIoGeneratorCfg {
    DirectoryCommonCfg output;
    DirectoryCommonCfg input;
};

struct DirectoryNestedGeneratorCfg {
    DirectoryCommonCfg overview;
    DirectoryCommonCfg nested;
};

struct DirectoryGeneratorCfg {
    DirectoryStaticIoGeneratorCfg networth_projector;
    DirectoryStaticIoGeneratorCfg mortgage_calculator;
    DirectoryNestedGeneratorCfg portfolio_manager;
};

#endif /* CONF_INPUT_TYPES_HPP_ */