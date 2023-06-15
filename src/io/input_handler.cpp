#include <string>
#include <iostream>
#include "input_handler.hpp"

template <typename T>
static void getGenericInputParam(T& input, const T defVal, std::string dispTxt) {
    const char newline = '\n' ;
    std::cout << "Enter " << dispTxt << " (or just enter to use default): " ;
    if( std::cin.peek() != newline && std::cin >> input ) {
        std::cin.ignore( 1000000, '\n' );
        return;
    }
    else {
        std::cin.ignore( 1000000, '\n' );
        input = defVal;
    }
}

void ConcreteNetworthProjector::fillDefaults() {
    m_init_nw = 10000;
    m_year_income = 49000;
    m_age_retirement = 62;
    m_current_age = 22;
    m_year_increase = 2;
    m_port_yearly_ret = 10;
    m_port_fees = 2;
    m_inv_yearly = 15;
}

void ConcreteNetworthProjector::getInputFromDefaults(InputDataContainer& input_data) {
    input_data.networth_projector.init_nw = m_init_nw;
    input_data.networth_projector.year_income = m_year_income;
    input_data.networth_projector.age_retirement = m_age_retirement;
    input_data.networth_projector.current_age = m_current_age;
    input_data.networth_projector.year_increase = m_year_increase;
    input_data.networth_projector.port_yearly_ret = m_port_yearly_ret;
    input_data.networth_projector.port_fees = m_port_fees;
    input_data.networth_projector.inv_yearly = m_inv_yearly;
    input_data.specifier = InputDataContainer::Specifier::NETWORTH_INPUT;
}

void ConcreteNetworthProjector::getInputFromUser(InputDataContainer& input_data) {
    getGenericInputParam(input_data.networth_projector.init_nw,
                         m_init_nw,
                         std::string("initial networth"));
    getGenericInputParam(input_data.networth_projector.year_income,
                         m_year_income,
                         std::string("yearly income"));
    getGenericInputParam(input_data.networth_projector.age_retirement,
                         m_age_retirement,
                         std::string("age of retirement"));
    getGenericInputParam(input_data.networth_projector.current_age,
                         m_current_age,
                         std::string("current age"));
    getGenericInputParam(input_data.networth_projector.year_increase,
                         m_year_increase,
                         std::string("yearly increase %"));
    getGenericInputParam(input_data.networth_projector.inv_yearly,
                         m_inv_yearly,
                         std::string("yearly investing %"));
    getGenericInputParam(input_data.networth_projector.port_yearly_ret,
                         m_port_yearly_ret,
                         std::string("portfolio return %"));
    getGenericInputParam(input_data.networth_projector.port_fees,
                         m_port_fees,
                         std::string("portfolio fees %"));
    input_data.specifier = InputDataContainer::Specifier::NETWORTH_INPUT;
}

void ConcreteNetworthProjector::getInputFromCfg(InputDataContainer& input_data) {
    std::cout << "Feature not yet implemented!" << std::flush;
    input_data.specifier = InputDataContainer::Specifier::NETWORTH_INPUT;
}

void ConcreteMortgageCalculator::fillDefaults() {
    m_price = 425000;
    m_num_months = 360;
    m_interest_rate = 4.f;
    m_market_increase = 3.f;
    m_rent_to_compare = 1800.f;
    m_makelaar_fees = 8000;
    m_rent_annu_increase = 8.f;
}

void ConcreteMortgageCalculator::getInputFromDefaults(InputDataContainer& input_data) {
    input_data.mortgage_calculator.price = m_price;
    input_data.mortgage_calculator.num_months = m_num_months;
    input_data.mortgage_calculator.interest_rate = m_interest_rate;
    input_data.mortgage_calculator.market_increase = m_market_increase;
    input_data.mortgage_calculator.rent_to_compare = m_rent_to_compare;
    input_data.mortgage_calculator.makelaar_fees = m_makelaar_fees;
    input_data.mortgage_calculator.rent_annu_increase = m_rent_annu_increase;
    input_data.specifier = InputDataContainer::Specifier::MORTGAGE_INPUT;
}

void ConcreteMortgageCalculator::getInputFromUser(InputDataContainer& input_data) {
    getGenericInputParam(input_data.mortgage_calculator.price,
                         m_price,
                         std::string("unit's original price"));
    getGenericInputParam(input_data.mortgage_calculator.num_months,
                         m_num_months,
                         std::string("number of months"));
    getGenericInputParam(input_data.mortgage_calculator.interest_rate,
                         m_interest_rate,
                         std::string("interest rate"));
    getGenericInputParam(input_data.mortgage_calculator.market_increase,
                         m_market_increase,
                         std::string("housing market inflation"));
    getGenericInputParam(input_data.mortgage_calculator.rent_to_compare,
                         m_rent_to_compare,
                         std::string("rent to compare (per month)"));
    getGenericInputParam(input_data.mortgage_calculator.makelaar_fees,
                         m_makelaar_fees,
                         std::string("initial expenses"));
    getGenericInputParam(input_data.mortgage_calculator.rent_annu_increase,
                         m_rent_annu_increase,
                         std::string("rent annual increase"));
    input_data.specifier = InputDataContainer::Specifier::MORTGAGE_INPUT;
}

void ConcreteMortgageCalculator::getInputFromCfg(InputDataContainer& input_data) {
    std::cout << "Feature not yet implemented!" << std::flush;
    input_data.specifier = InputDataContainer::Specifier::MORTGAGE_INPUT;
}

void CreatorInput::getDataFromUser(InputDataContainer& input_data) const {
    Input* input = this->FactoryMethod();
    input->fillDefaults();
    std::string user_selection;
    std::cout << "Welcome to data source selection.\n";
    std::cout << "Select one of the following:\n";
    std::cout << "1. Manually entering your data.\n";
    std::cout << "2. Fill your data in configurations file.\n";
    std::cout << "3. Use predefined data.\n";
    std::cout << "Selection: ";
    std::cin >> user_selection;
    if (user_selection == "1") {
        std::cin.ignore( 1000000, '\n' );
        input->getInputFromUser(input_data);
    }
    else if (user_selection == "2") {
        std::cin.ignore( 1000000, '\n' );
        input->getInputFromCfg(input_data);
    }
    else if (user_selection == "3") {
        std::cin.ignore( 1000000, '\n' );
        input->getInputFromDefaults(input_data);
    }
    else {
        std::cin.ignore(1000000, '\n');
        std::cout << "Unknown selection!\n";
    }
    delete input;
}

static void getUserInputData(const CreatorInput& input,
                             InputDataContainer& input_data) {
    input.getDataFromUser(input_data);
}

void getUserSelection(InputDataContainer& input_data) {
    std::string input_ss;
    std::cout << "Choose mode of operation, from the following list:\n";
    std::cout << "n: networth projection\n";
    std::cout << "m: mortgage calculation\n";
    std::cout << "p: portfolio management\n";
    std::cout << "Selection: ";
    std::cin >> input_ss;
    if (input_ss == "n" || input_ss == "N") {
        std::cin.ignore( 1000000, '\n' );
        CreatorInput* creator_input = new ConcreteCreatorNetworthProjector();
        getUserInputData(*creator_input, input_data);
    }
    else if (input_ss == "m" || input_ss == "M") {
        std::cin.ignore( 1000000, '\n' );
        CreatorInput* creator_input = new ConcreteCreatorMortgageCalculator();
        getUserInputData(*creator_input, input_data);
    }
    else if (input_ss == "p" || input_ss == "P") {
        std::cin.ignore( 1000000, '\n' );
        std::cout << "Feature not yet implemented!" << std::flush;
    }
    else {
        std::cout << "Unknown selection!\n";
    }
}