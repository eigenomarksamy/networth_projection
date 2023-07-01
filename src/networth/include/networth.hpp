#ifndef NETWORTH_HPP_
#define NETWORTH_HPP_

#include <math.h>
#include <array>
#include <string>
#include <vector>
#include <unordered_map>

namespace networth {

class NetWorth {

public:
    struct Percentages{
        float_t yearly_income_increase;
        float_t portfolio_yearly_return;
        float_t portfolio_fees;
        float_t investing;
    };

    enum ATTR_DESC {
        CUR_AGE = 0,
        CUR_YEAR,
        INCOME,
        YEARLY_INV,
        DEP_OR_WD,
        GROSS_INV_RET,
        GROSS_NW,
        NET_INV_RET,
        NET_NW,
        ATTR_LEN
    };

    NetWorth (uint32_t nw, uint32_t yearly_income, uint8_t age_retirement,
              uint8_t cur_age, Percentages percentages);

    void printTabulatedData() const;

    void computeData();

    void setDepWithdrawalPlan(const std::unordered_map<uint32_t, float_t>& plan) {
        this->m_dep_wd_plan = plan;
    }

    std::vector<std::array<float_t, ATTR_LEN>> getData() const { return m_data; }

    std::vector<std::string> getDataNames() const;

private:
    uint32_t m_init_nw;
    uint32_t m_yearly_income;
    uint8_t m_age_retirement;
    uint8_t m_cur_year;
    uint8_t m_cur_age;
    uint8_t m_init_age;
    Percentages m_percentages;
    std::vector<std::array<float_t, ATTR_LEN>> m_data;
    std::array<float_t, ATTR_LEN> m_attr_arr;
    std::unordered_map<uint32_t, float_t> m_dep_wd_plan;

};

} // namespace networth

#endif /* NETWORTH_HPP_ */