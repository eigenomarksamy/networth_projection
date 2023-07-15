#ifndef APPL_CONF_TYPES_HPP_
#define APPL_CONF_TYPES_HPP_

#include "conf_resolver.hpp"

struct DirectoriesValues {
    std::shared_ptr<config_elm_t> mortg_calc_in;
    std::shared_ptr<config_elm_t> mortg_calc_out;
    std::shared_ptr<config_elm_t> netwo_calc_in;
    std::shared_ptr<config_elm_t> netwo_calc_out;
    std::shared_ptr<config_elm_t> porto_dirs_out;
    std::shared_ptr<config_elm_t> porto_overview;
};

struct NetworthValues {
    std::shared_ptr<config_elm_t> initial_networth;
    std::shared_ptr<config_elm_t> yearly_income;
    std::shared_ptr<config_elm_t> age_retirement;
    std::shared_ptr<config_elm_t> age_current;
    std::shared_ptr<config_elm_t> yearly_income_raise;
    std::shared_ptr<config_elm_t> yearly_roi;
    std::shared_ptr<config_elm_t> yearly_investment;
    std::shared_ptr<config_elm_t> portfolio_fees;
    std::vector<std::shared_ptr<config_elm_t>> deposit_withdrawal;
};

struct MortgageValues {
    std::shared_ptr<config_elm_t> original_price;
    std::shared_ptr<config_elm_t> time_to_settle_months;
    std::shared_ptr<config_elm_t> time_to_settle_years;
    std::shared_ptr<config_elm_t> interest_rate;
    std::shared_ptr<config_elm_t> market_inflation;
    std::shared_ptr<config_elm_t> initial_overhead;
    std::shared_ptr<config_elm_t> rent_to_compare;
    std::shared_ptr<config_elm_t> annual_rent_increase;
};

#endif /* APPL_CONF_TYPES_HPP_ */