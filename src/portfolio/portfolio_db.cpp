#include "portfolio_db.hpp"

bool portfolio::DatabaseInterfaceImplementation::saveInvestment(const Investment& investment) {
    return true;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentQuantity(const std::string& ticker, const uint32_t quantity) {
    return true;
}

bool portfolio::DatabaseInterfaceImplementation::updateInvestmentPrice(const std::string& ticker, const double_t price) {
    return true;
}

bool portfolio::DatabaseInterfaceImplementation::removeInvestment(const std::string& ticker) {
    return true;
}

uint32_t portfolio::DatabaseInterfaceImplementation::getInvestmentQuantity(const std::string& ticker) {
    return 0;
}

double_t portfolio::DatabaseInterfaceImplementation::getInvestmentPrice(const std::string& ticker) {
    return 0.;
}