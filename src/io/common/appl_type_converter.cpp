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
    // input.num_months = convertToNumeric<decltype(input.interest_rate)>(values.interest_rate->value);
}