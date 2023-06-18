#include <string>
#include <iostream>
#include "utils.hpp"
#include <sstream>
#include <cstring>
#include "input_handler.hpp"

enum class GenericInputMode { SingleValueSingleData,
                              MultipleValuesSingleData,
                              MultipleValuesMultipleData };

std::ostream& operator<<(std::ostream& out, const GenericInputMode& inputMode) {
    switch (inputMode) {
        case GenericInputMode::SingleValueSingleData: out << "Single Value Single Data"; break;
        case GenericInputMode::MultipleValuesSingleData: out << "Multiple Values Single Data"; break;
        case GenericInputMode::MultipleValuesMultipleData: out << "Multiple Values Multiple Data"; break;
    }
    return out;
}

template <typename T>
static void getGenericInputParam(T& input, const T defVal, std::string dispTxt) {
    constexpr int8_t newline = '\n' ;
    constexpr uint32_t ignoreVal = 1000000;
    std::cout << "Enter parameter '" << dispTxt << "', input mode: '"
              << GenericInputMode::SingleValueSingleData
              << "' (you can leave blank to use default): " ;
    if( std::cin.peek() != newline && std::cin >> input ) {
        std::cin.ignore( ignoreVal, newline );
        return;
    }
    else {
        std::cin.ignore( ignoreVal, newline );
        input = defVal;
    }
}

template <typename T>
static void getGenericInputParam(std::vector<T>& input,
                                 const std::string& dispTxt) {
    constexpr int8_t newline = '\n' ;
    constexpr uint32_t ignoreVal = 1000000;
    T inputElm;
    std::cout << "Enter parameter '" << dispTxt << "', input mode: '"
              << GenericInputMode::MultipleValuesSingleData
              << "' (you can leave blank to use default): " ;
    while ( std::cin.peek() != newline && std::cin >> inputElm ) {
        input.push_back(inputElm);
    }
    std::cin.ignore( ignoreVal, newline );
}

template <typename T>
static void getGenericInputParam(std::vector<std::vector<T>>& input,
                                 const std::string& dispTxt) {
    constexpr int8_t newline = '\n' ;
    constexpr uint32_t ignoreVal = 1000000;
    T inputElm;
    std::vector<decltype(inputElm)> inputVec;
    std::string inputString;
    std::vector<std::string> vectInput;
    std::cout << "Enter parameter '" << dispTxt << "', input mode: '"
              << GenericInputMode::MultipleValuesMultipleData
              << "' (you can leave blank to use default): " ;
    while (std::cin.peek() != newline) {
        std::cin >> inputString;
        vectInput.push_back(inputString);
    }
    for (auto& str : vectInput) {
        inputVec.clear();
        std::stringstream ss(str);
        for (; ss >> inputElm;) {
            inputVec.push_back(inputElm);
            if (ss.peek() == ',')
                ss.ignore();
        }
        input.push_back(inputVec);
    }
    std::cin.ignore( ignoreVal, newline );
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
    getGenericInputParam(input_data.networth_projector.year_to_amount,
                         "year and deposit/withdrawal");
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

void ConcretePortfolioManager::fillDefaults() {
    m_is_new = true;
    m_is_multi_prtfolio = true;
    m_load_all_portfolios = true;
    m_name = "MyInvestmentPortfolio_" + getLocalDateTime();
}

void ConcretePortfolioManager::getInputFromDefaults(InputDataContainer& input_data) {
    input_data.portfolio_manager.is_new = m_is_new;
    input_data.portfolio_manager.is_multi_prtfolio = m_is_multi_prtfolio;
    input_data.portfolio_manager.name = m_name;
    input_data.specifier = InputDataContainer::Specifier::PORTFOLIO_INPUT;
}

void ConcretePortfolioManager::getInputFromUser(InputDataContainer& input_data) {
    getGenericInputParam(input_data.portfolio_manager.is_new,
                         m_is_new,
                         std::string("create new profile"));
    getGenericInputParam(input_data.portfolio_manager.is_multi_prtfolio,
                         m_is_multi_prtfolio,
                         std::string("multi portfolio mode"));
    if (!input_data.portfolio_manager.is_multi_prtfolio) {
        getGenericInputParam(input_data.portfolio_manager.name,
                            m_name,
                            std::string("name of portfolio"));
    }
    if (!input_data.portfolio_manager.is_new
        && input_data.portfolio_manager.is_multi_prtfolio) {
        getGenericInputParam(input_data.portfolio_manager.load_all_portfolios,
                            m_load_all_portfolios,
                            std::string("load all portfolios"));
        if (!input_data.portfolio_manager.load_all_portfolios) {
            getGenericInputParam(input_data.portfolio_manager.portfolio_list,
                                std::string("portfolios names"));
        }
    }
    input_data.specifier = InputDataContainer::Specifier::PORTFOLIO_INPUT;
}

void ConcretePortfolioManager::getInputFromCfg(InputDataContainer& input_data) {
    std::cout << "Feature not yet implemented!" << std::flush;
    input_data.specifier = InputDataContainer::Specifier::PORTFOLIO_INPUT;
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
        std::cout << "Usage modes for manual input:\n";
        std::cout << GenericInputMode::SingleValueSingleData;
        std::cout << ": Enter a single value, then new line to move on.\n";
        std::cout << GenericInputMode::MultipleValuesSingleData;
        std::cout << ": Enter multiple single values (space seperated), "
                  << "and new line to finish and move on.\n";
        std::cout << GenericInputMode::MultipleValuesMultipleData;
        std::cout << ": Enter multiple multiple values, "
                  << "comma separed for one set and "
                  << "space seperated for different sets, "
                  << "and new line to finish and move on.\n";
        std::cout << std::endl;
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
        CreatorInput* creator_input = new ConcreteCreatorPortfolioManager();
        getUserInputData(*creator_input, input_data);
    }
    else {
        std::cout << "Unknown selection!\n";
    }
}