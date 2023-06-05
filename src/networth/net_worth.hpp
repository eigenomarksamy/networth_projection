#ifndef NET_WORTH_HPP_
#define NET_WORTH_HPP_

#include <array>
#include <vector>

class NetWorth {

public:
    struct Percentages{
        float yearly_income_increase;
        float portfolio_yearly_return;
        float portfolio_fees;
        float investing;
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

    NetWorth (int nw, int yearly_income, int age_retirement,
              int cur_age, Percentages percentages);

    void printTabulatedData() const;

    void computeData();

    std::vector<std::array<float, ATTR_LEN>> getData() const { return m_data; }

private:
    int m_init_nw;
    int m_yearly_income;
    int m_age_retirement;
    int m_cur_year;
    int m_cur_age;
    int m_init_age;
    Percentages m_percentages;
    std::vector<std::array<float, ATTR_LEN>> m_data;
    std::array<float, ATTR_LEN> m_attr_arr;

};

#endif /* NET_WORTH_HPP_ */