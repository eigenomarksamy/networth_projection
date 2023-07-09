#ifndef MORTGAGE_HPP_
#define MORTGAGE_HPP_

#include <math.h>
#include <stdint.h>
#include <vector>
#include <array>
#include <string>
#include "str_adapter.hpp"
#include "computation_adapter.hpp"

namespace mortgage {

class Mortgage : public StaticComputation {

    enum Mortgage_Attr_Desc {
        PERIODE = 0,
        TOTALE_SCHULD,
        TE_BETALEN_RENTE,
        TE_BETALEN_AFLOSSING,
        RESTSCHULD,
        TOTAAL_BETAALDE_HUUR,
        TOTAAL_BETAALDE_RENTE,
        HUUR_NETTO_WINST,
        LANDGOED_WAARDE,
        WINST_VERKOPEN,
        ATTR_DESC_LEN
    };

    float_t computeInterestToBePaid() const;
    float_t computeRepayment() const;
    float_t computeResidualDebt() const;
    float_t computeTotalGainForRent() const;
    float_t computeEstateMarketValue() const;
    float_t computeSellingGain() const;
    float_t computeCurrentRent() const;

    uint32_t m_price;
    uint16_t m_num_months;
    float_t m_interest_rate;
    float_t m_market_increase;
    float_t m_rent_to_compare;
    uint16_t m_makelaar_fees;
    float_t m_rent_annu_increase;
    float_t m_totale_maandbedrag;
    std::vector<std::array<float_t, Mortgage_Attr_Desc::ATTR_DESC_LEN>> m_data;
    std::array<float_t, Mortgage_Attr_Desc::ATTR_DESC_LEN> m_attr_arr;

public:

    Mortgage(uint32_t price, uint16_t num_months,
             float_t interest_rate, float_t market_increase,
             float_t rent_to_compare, uint16_t makelaar_fees,
             float_t rent_annu_increase)
        : m_price(price), m_num_months(num_months),
          m_interest_rate(interest_rate), m_market_increase(market_increase),
          m_rent_to_compare(rent_to_compare), m_makelaar_fees(makelaar_fees),
          m_rent_annu_increase(rent_annu_increase) { }

    void computeData() override;
    void printTabulatedData() const override;
    std::vector<std::string> getDataNames() const override;
    auto getData() const -> decltype(m_data) { return m_data; }
    uint32_t getPrice() const { return m_price; }
    uint16_t getNumMonths() const { return m_num_months; }
    uint16_t getInitialFees() const { return m_makelaar_fees; }
    float_t getInterestRate() const { return m_interest_rate; }
    float_t getMarketIncrease() const { return m_market_increase; }
    float_t getRentToCompare() const { return m_rent_to_compare; }
    float_t getRentAnnualIncrease() const { return m_rent_annu_increase; }
};

class MortgageAdapter : public StrStreamGenerator {
private:
    Mortgage m_mortgageObj;

public:
    MortgageAdapter(const Mortgage& mortgageObj) : m_mortgageObj(mortgageObj) {}
    std::vector<std::string> generateDataNames() const override;
    std::vector<std::vector<std::string>> generateDataLines() const override;
    std::vector<std::string> generateInputPreview() const override;

};

} // namespace mortgage

#endif /* MORTGAGE_HPP_ */