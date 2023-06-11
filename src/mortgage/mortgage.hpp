#ifndef MORTGAGE_HPP_
#define MORTGAGE_HPP_

#include <math.h>
#include <stdint.h>
#include <vector>
#include <array>
#include <string>

class Mortgage {

    enum Mortgage_Attr_Desc {
        PERIODE = 0,
        TOTALE_SCHULD,
        TE_BETALEN_RENTE,
        TE_BETALEN_AFLOSSING,
        RESTSCHULD,
        ATTR_DESC_LEN
    };

    float_t computeInterestToBePaid() const;
    float_t computeRepayment() const;
    float_t computeResidualDebt() const;

    uint32_t m_price;
    uint16_t m_num_months;
    float_t m_interest_rate;
    float_t m_market_increase;
    float_t m_rent_to_compare;
    float_t m_totale_maandbedrag;
    std::vector<std::array<float_t, Mortgage_Attr_Desc::ATTR_DESC_LEN>> m_data;
    std::array<float_t, Mortgage_Attr_Desc::ATTR_DESC_LEN> m_attr_arr;

public:

    Mortgage(uint32_t price, uint16_t num_months,
             float_t interest_rate, float_t market_increase,
             float_t rent_to_compare)
        : m_price(price), m_num_months(num_months),
          m_interest_rate(interest_rate), m_market_increase(market_increase),
          m_rent_to_compare(rent_to_compare) { }

    void computeData();
    void printTabulatedData() const;
    auto getData() const -> decltype(m_data) { return m_data; }
    std::vector<std::string> getDataNames() const;
};

#endif /* MORTGAGE_HPP_ */