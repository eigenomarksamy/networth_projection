#include <iostream>
#include "mortgage.hpp"

void Mortgage::computeData() {
    
}

void Mortgage::printTabulatedData() const {
    std::cout << "periode\t\ttotale schuld\t\tte betalen rente\t\t"
              << "te betalen aflossing\t\trestschuld\t\ttotale maandbedrag\t\t";
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
    retVecStr[TOTALE_MAANDBEDRAGE] = "Totale maandbedrag";
    return retVecStr;
}