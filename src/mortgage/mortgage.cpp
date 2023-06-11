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
    m_data.push_back(m_attr_arr);
    m_attr_arr[PERIODE]++;
    for (; m_attr_arr[PERIODE] <= m_num_months; ++m_attr_arr[PERIODE]) {
        m_attr_arr[TOTALE_SCHULD] -= m_attr_arr[TE_BETALEN_AFLOSSING];
        m_attr_arr[TE_BETALEN_RENTE] = computeInterestToBePaid();
        m_attr_arr[TE_BETALEN_AFLOSSING] = computeRepayment();
        m_attr_arr[RESTSCHULD] = computeResidualDebt();
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

void Mortgage::printTabulatedData() const {
    std::cout << "periode\t\ttotale schuld\t\tte betalen rente\t\t"
              << "te betalen aflossing\t\trestschuld\t\t\n";
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
    return retVecStr;
}