#include <iostream>
#include "mortgage.hpp"
#include "utils.hpp"

void Mortgage::computeData() {
    m_attr_arr[PERIODE] = 1;
    m_attr_arr[TOTALE_SCHULD] = m_price;
    m_attr_arr[TE_BETALEN_RENTE] = computeInterestToBePaid();
    m_totale_maandbedrag = -computePMT(m_price, getPercent(m_interest_rate), m_num_months);
    m_attr_arr[TE_BETALEN_AFLOSSING] = computeRepayment();
    m_attr_arr[RESTSCHULD] = computeResidualDebt();
    m_attr_arr[TOTAAL_BETAALDE_HUUR] = m_rent_to_compare;
    m_attr_arr[TOTAAL_BETAALDE_RENTE] = m_attr_arr[TE_BETALEN_RENTE];
    m_attr_arr[HUUR_NETTO_WINST] = computeTotalGainForRent();
    m_attr_arr[LANDGOED_WAARDE] = m_price;
    m_attr_arr[WINST_VERKOPEN] = computeSellingGain();
    m_data.push_back(m_attr_arr);
    m_attr_arr[PERIODE]++;
    for (; m_attr_arr[PERIODE] <= m_num_months; ++m_attr_arr[PERIODE]) {
        m_attr_arr[TOTALE_SCHULD] -= m_attr_arr[TE_BETALEN_AFLOSSING];
        m_attr_arr[TE_BETALEN_RENTE] = computeInterestToBePaid();
        m_attr_arr[TE_BETALEN_AFLOSSING] = computeRepayment();
        m_attr_arr[RESTSCHULD] = computeResidualDebt();
        m_attr_arr[TOTAAL_BETAALDE_HUUR] += computeCurrentRent();
        m_attr_arr[TOTAAL_BETAALDE_RENTE] += m_attr_arr[TE_BETALEN_RENTE];
        m_attr_arr[HUUR_NETTO_WINST] = computeTotalGainForRent();
        m_attr_arr[LANDGOED_WAARDE] = computeEstateMarketValue();
        m_attr_arr[WINST_VERKOPEN] = computeSellingGain();
        m_data.push_back(m_attr_arr);
    }
}

float_t Mortgage::computeInterestToBePaid() const {
    return m_attr_arr[TOTALE_SCHULD] * (getPercent(m_interest_rate) / 12.f);
}

float_t Mortgage::computeRepayment() const {
    return m_totale_maandbedrag - m_attr_arr[TE_BETALEN_RENTE];
}

float_t Mortgage::computeResidualDebt() const {
    return m_attr_arr[TOTALE_SCHULD] - m_attr_arr[TE_BETALEN_AFLOSSING];
}

float_t Mortgage::computeTotalGainForRent() const {
    return m_attr_arr[TOTAAL_BETAALDE_RENTE] + m_makelaar_fees - m_attr_arr[TOTAAL_BETAALDE_HUUR];
}

float_t Mortgage::computeEstateMarketValue() const {
    return m_attr_arr[LANDGOED_WAARDE] * (1.f + getPercent(m_market_increase) / 12.f);
}

float_t Mortgage::computeSellingGain() const {
    return m_attr_arr[LANDGOED_WAARDE] - m_attr_arr[TOTALE_SCHULD] - m_makelaar_fees;
}

float_t Mortgage::computeCurrentRent() const {
    return m_rent_to_compare * (floorf((m_attr_arr[PERIODE] - 1) / 12) * getPercent(m_rent_annu_increase) + 1.f);
}

void Mortgage::printTabulatedData() const {
    std::cout << "periode\ttotale schuld\tte betalen rente\t"
              << "te betalen aflossing\trestschuld\ttotaal betaalde huur\t"
              << "totaal betaalde rente\thuur netto winst\tlandgoed waarde\t"
              << "winst verkopen\n";
    for (const auto& d : m_data) {
        for (const auto& a : d) {
            std::cout <<std::fixed << a << "\t";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> Mortgage::getDataNames() const {
    std::vector<std::string> retVecStr(ATTR_DESC_LEN);
    retVecStr[PERIODE] = "Periode";
    retVecStr[TOTALE_SCHULD] = "Totale schuld";
    retVecStr[TE_BETALEN_RENTE] = "Te betalen rente";
    retVecStr[TE_BETALEN_AFLOSSING] = "Te betalen aflossing";
    retVecStr[RESTSCHULD] = "Restschuld";
    retVecStr[TOTAAL_BETAALDE_HUUR] = "Totaal betaalde huur";
    retVecStr[TOTAAL_BETAALDE_RENTE] = "Totaal betaalde rente";
    retVecStr[HUUR_NETTO_WINST] = "Huur netto winst";
    retVecStr[LANDGOED_WAARDE] = "Landgoed waarde";
    retVecStr[WINST_VERKOPEN] = "Winst verkopen";
    return retVecStr;
}