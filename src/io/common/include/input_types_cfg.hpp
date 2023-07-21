#ifndef INPUT_TYPES_CFG_HPP_
#define INPUT_TYPES_CFG_HPP_

#include <math.h>
#include <string>
#include <vector>
#include "utils.hpp"

struct InputDataNetworthProjector {
    uint32_t init_nw = 10000;
    uint32_t year_income = 49000;
    uint32_t age_retirement = 62;
    uint32_t current_age = 22;
    float_t year_increase = 2;
    float_t port_yearly_ret = 10;
    float_t port_fees = 2;
    float_t inv_yearly = 15;
    std::vector<std::vector<float_t>> year_to_amount;
};

struct InputDataMortgageCalculator {
    uint32_t price = 425000;
    uint16_t num_months = 360;
    float_t interest_rate = 4.f;
    float_t market_increase = 3.f;
    float_t rent_to_compare = 1800.f;
    uint16_t makelaar_fees = 8000;
    float_t rent_annu_increase = 8.f;
};

struct InputPortfolioManager {
    bool is_new = true;
    bool is_multi_prtfolio = true;
    bool load_all_portfolios = true;
    bool auto_save = true;
    std::string name = "MyInvestmentPortfolio_" + getLocalDateTime();
    std::vector<std::string> portfolio_list;
};

struct InputDataContainer {
    enum Specifier {
        NONE = 0,
        NETWORTH_INPUT,
        MORTGAGE_INPUT,
        PORTFOLIO_INPUT
    };
    InputDataNetworthProjector networth_projector;
    InputDataMortgageCalculator mortgage_calculator;
    InputPortfolioManager portfolio_manager;
    Specifier specifier;
};

#endif /* INPUT_TYPES_CFG_HPP_ */