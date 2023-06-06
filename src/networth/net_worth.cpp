#include <iostream>
#include "utils.hpp"
#include "net_worth.hpp"

NetWorth::NetWorth (int nw, int yearly_income, int age_retirement,
              int cur_age, Percentages percentages)
              : m_init_nw(nw), m_yearly_income(yearly_income),
                m_age_retirement(age_retirement), m_cur_age(cur_age),
                m_percentages(percentages) {
    m_cur_year = 1;
    m_init_age = m_cur_age;
    m_attr_arr[CUR_AGE] = m_cur_age;
    m_attr_arr[CUR_YEAR] = m_cur_year;
    m_attr_arr[INCOME] = m_yearly_income;
    m_attr_arr[YEARLY_INV] = m_attr_arr[INCOME] * getPercent(m_percentages.investing);
    m_attr_arr[DEP_OR_WD] = 0;
    m_attr_arr[GROSS_INV_RET] = m_init_nw * getPercent(m_percentages.portfolio_yearly_return) + m_attr_arr[YEARLY_INV] * getPercent(m_percentages.portfolio_yearly_return) / 2 + m_attr_arr[DEP_OR_WD] * getPercent(m_percentages.portfolio_yearly_return) / 2;
    m_attr_arr[GROSS_NW] = m_init_nw + m_attr_arr[YEARLY_INV] + m_attr_arr[GROSS_INV_RET];
    m_attr_arr[NET_INV_RET] = m_init_nw * (getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees)) + m_attr_arr[YEARLY_INV] * ((getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees)) / 2) + m_attr_arr[DEP_OR_WD] * ((getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees)) / 2);
    m_attr_arr[NET_NW] = m_init_nw + m_attr_arr[YEARLY_INV] + m_attr_arr[NET_INV_RET];
    m_data.push_back(m_attr_arr);
    m_cur_age++;
}

void NetWorth::computeData() {
    for (; m_cur_age <= m_age_retirement; m_cur_age++) {
        m_attr_arr[CUR_AGE] = m_cur_age;
        m_attr_arr[CUR_YEAR] = m_cur_age - m_init_age + 1;
        m_attr_arr[INCOME] *= (1 + getPercent(m_percentages.yearly_income_increase));
        m_attr_arr[YEARLY_INV] = m_attr_arr[INCOME] * getPercent(m_percentages.investing);
        m_attr_arr[GROSS_INV_RET] = m_attr_arr[GROSS_NW] * getPercent(m_percentages.portfolio_yearly_return) + m_attr_arr[YEARLY_INV] *  getPercent(m_percentages.portfolio_yearly_return) / 2 + m_attr_arr[DEP_OR_WD] * getPercent(m_percentages.portfolio_yearly_return) / 2;
        m_attr_arr[GROSS_NW] += m_attr_arr[YEARLY_INV] + m_attr_arr[DEP_OR_WD] + m_attr_arr[GROSS_INV_RET];
        m_attr_arr[NET_INV_RET] = m_attr_arr[NET_NW] * (getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees)) + m_attr_arr[YEARLY_INV] * (((getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees))) / 2) + m_attr_arr[DEP_OR_WD] * ((getPercent(m_percentages.portfolio_yearly_return) - getPercent(m_percentages.portfolio_fees)) / 2);
        m_attr_arr[NET_NW] += m_attr_arr[YEARLY_INV] + m_attr_arr[DEP_OR_WD] + m_attr_arr[NET_INV_RET];
        m_data.push_back(m_attr_arr);
    }
}

void NetWorth::printTabulatedData() const {
    std::cout << "age\t\tyear\t\tincome\t\tyearly investment\t\t"
              << "deposits/withdrawals\t\tgross return\t\t"
              << "gross networth\t\tnet return\t\tnet networth\n";
    for (const auto& d : m_data) {
        for (const auto& a : d) {
            std::cout <<std::fixed << a << "\t";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> NetWorth::getDataNames() const {
    std::vector<std::string> retVecStr(ATTR_LEN);
    retVecStr[CUR_AGE] = "Age";
    retVecStr[CUR_YEAR] = "Year";
    retVecStr[INCOME] = "Income";
    retVecStr[YEARLY_INV] = "Yearly Investment";
    retVecStr[DEP_OR_WD] = "Deposits/Withdrawals";
    retVecStr[GROSS_INV_RET] = "Investment return this year";
    retVecStr[GROSS_NW] = "Net worth";
    retVecStr[NET_INV_RET] = "Investment return this year (after fees)";
    retVecStr[NET_NW] = "Net worth (after fees)";
    return retVecStr;
}