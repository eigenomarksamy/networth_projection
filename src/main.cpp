#include "NetWorth.hpp"

int main() {
    int init_nw = 10000, year_income = 49000;
    float year_increase = 2, prot_yearly_ret = 10, port_fees = 2, perc_inv = 15;
    int age_retirement = 62, cur_year = 1, current_age = 22;
    NetWorth networth(init_nw, year_income, age_retirement, current_age, NetWorth::Percentages({year_increase, prot_yearly_ret, port_fees, perc_inv}));
    networth.computeData();
    networth.printTabulatedData();
    return 0;
}