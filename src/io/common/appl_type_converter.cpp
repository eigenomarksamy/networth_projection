#include "appl_conf_types.hpp"
#include "utils.hpp"

void convertNetworthYmlData(InputDataNetworthProjector& input, const NetworthValues& values) {
    input.age_retirement = convertToNumeric<decltype(input.age_retirement)>(values.age_retirement->value);
    input.port_fees = convertToNumeric<decltype(input.port_fees)>(values.portfolio_fees->value);
    input.current_age = convertToNumeric<decltype(input.current_age)>(values.age_current->value);
    input.init_nw = convertToNumeric<decltype(input.init_nw)>(values.initial_networth->value);
    input.init_nw = convertToNumeric<decltype(input.year_income)>(values.yearly_income->value);
    input.init_nw = convertToNumeric<decltype(input.year_increase)>(values.yearly_income_raise->value);
    input.init_nw = convertToNumeric<decltype(input.inv_yearly)>(values.yearly_investment->value);
    input.init_nw = convertToNumeric<decltype(input.port_yearly_ret)>(values.yearly_roi->value);
    for (const auto& itr : values.deposit_withdrawal) {
        std::vector<std::string> one_relation;
        std::vector<std::remove_const_t<std::remove_reference_t<decltype(input.year_to_amount.front().front())>>> one_relation_numeric;
        one_relation.push_back(itr->name);
        one_relation.push_back(itr->value);
        for (const auto& i : one_relation) {
            one_relation_numeric.push_back(convertToNumeric<std::remove_const_t<std::remove_reference_t<decltype(one_relation_numeric.front())>>>(i));
        }
        input.year_to_amount.push_back(one_relation_numeric);
    }
}

void convertMortgageYmlData(InputDataMortgageCalculator& input, const MortgageValues& values) {
    input.interest_rate = convertToNumeric<decltype(input.interest_rate)>(values.interest_rate->value);
    input.makelaar_fees = convertToNumeric<decltype(input.makelaar_fees)>(values.initial_overhead->value);
    input.market_increase = convertToNumeric<decltype(input.market_increase)>(values.market_inflation->value);
    input.price = convertToNumeric<decltype(input.price)>(values.original_price->value);
    input.rent_annu_increase = convertToNumeric<decltype(input.rent_annu_increase)>(values.annual_rent_increase->value);
    input.rent_to_compare = convertToNumeric<decltype(input.rent_to_compare)>(values.rent_to_compare->value);
    input.num_months = convertToNumeric<decltype(input.num_months)>(values.time_to_settle_months->value);
    input.num_months += convertToNumeric<decltype(input.num_months)>(values.time_to_settle_years->value) * 12;
}

bool resolveCfg(const std::string& confPath, DirectoriesValues& dirs) {
    YmlCfg dir_cfg(confPath);
    dirs.mortg_calc_in = YmlCfg::createConfigElm("generation.mortgage-calculator.input");
    dir_cfg.addConfigElement(dirs.mortg_calc_in);
    dirs.netwo_calc_in = YmlCfg::createConfigElm("generation.networth-projector.input");
    dir_cfg.addConfigElement(dirs.netwo_calc_in);
    dirs.mortg_calc_out = YmlCfg::createConfigElm("generation.mortgage-calculator.output");
    dir_cfg.addConfigElement(dirs.mortg_calc_out);
    dirs.netwo_calc_out = YmlCfg::createConfigElm("generation.networth-projector.output");
    dir_cfg.addConfigElement(dirs.netwo_calc_out);
    if (dir_cfg.readCfg(false, false)) {
        return true;
    }
    return false;
}

bool resolveCfg(const std::string& confPath, NetworthValues& values) {
    YmlCfg cfg(confPath);
    values.age_current = YmlCfg::createConfigElm("age.current");
    cfg.addConfigElement(values.age_current);
    values.age_retirement = YmlCfg::createConfigElm("age.retirement");
    cfg.addConfigElement(values.age_retirement);
    values.initial_networth = YmlCfg::createConfigElm("initial-networth");
    cfg.addConfigElement(values.initial_networth);
    values.portfolio_fees = YmlCfg::createConfigElm("yearly.percentage.portfolio.fees");
    cfg.addConfigElement(values.portfolio_fees);
    values.yearly_income = YmlCfg::createConfigElm("yearly.income");
    cfg.addConfigElement(values.yearly_income);
    values.yearly_income_raise = YmlCfg::createConfigElm("yearly.percentage.income-raise");
    cfg.addConfigElement(values.yearly_income_raise);
    values.yearly_investment = YmlCfg::createConfigElm("yearly.percentage.investment");
    cfg.addConfigElement(values.yearly_investment);
    values.yearly_roi = YmlCfg::createConfigElm("yearly.percentage.portfolio.roi");
    cfg.addConfigElement(values.yearly_roi);
    values.deposit_withdrawal = YmlCfg::createConfigElm("deposit-withdrawal", confPath);
    cfg.addConfigElement(values.deposit_withdrawal);
    if (cfg.readCfg(true, false)) {
        return true;
    }
    return false;
}

bool resolveCfg(const std::string& confPath, MortgageValues& values) {
    YmlCfg cfg(confPath);
    values.annual_rent_increase = YmlCfg::createConfigElm("rent-to-compare.annual-increase");
    cfg.addConfigElement(values.annual_rent_increase);
    values.initial_overhead = YmlCfg::createConfigElm("price.overhead");
    cfg.addConfigElement(values.initial_overhead);
    values.interest_rate = YmlCfg::createConfigElm("interest-rate");
    cfg.addConfigElement(values.interest_rate);
    values.market_inflation = YmlCfg::createConfigElm("market-inflation");
    cfg.addConfigElement(values.market_inflation);
    values.original_price = YmlCfg::createConfigElm("price.unit");
    cfg.addConfigElement(values.original_price);
    values.rent_to_compare = YmlCfg::createConfigElm("rent-to-compare.monthly");
    cfg.addConfigElement(values.rent_to_compare);
    values.time_to_settle_months = YmlCfg::createConfigElm("time-to-settle.months");
    cfg.addConfigElement(values.time_to_settle_months);
    values.time_to_settle_years = YmlCfg::createConfigElm("time-to-settle.years");
    cfg.addConfigElement(values.time_to_settle_years);
    if (cfg.readCfg(true, true)) {
        return true;
    }
    return false;
}