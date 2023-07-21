#include "cmd_common.hpp"
#include "input_factory.hpp"

void ConcreteNetworthProjector::fillSpecifier(InputDataContainer& input_data) {
    input_data.specifier = InputDataContainer::Specifier::NETWORTH_INPUT;
}

void ConcreteNetworthProjector::getInputFromUser(InputDataContainer& input_data) {
    getGenericInputParam(input_data.networth_projector.init_nw,
                         std::string("initial networth"));
    getGenericInputParam(input_data.networth_projector.year_income,
                         std::string("yearly income"));
    getGenericInputParam(input_data.networth_projector.age_retirement,
                         std::string("age of retirement"));
    getGenericInputParam(input_data.networth_projector.current_age,
                         std::string("current age"));
    getGenericInputParam(input_data.networth_projector.year_increase,
                         std::string("yearly increase %"));
    getGenericInputParam(input_data.networth_projector.inv_yearly,
                         std::string("yearly investing %"));
    getGenericInputParam(input_data.networth_projector.port_yearly_ret,
                         std::string("portfolio return %"));
    getGenericInputParam(input_data.networth_projector.port_fees,
                         std::string("portfolio fees %"));
    getGenericInputParam(input_data.networth_projector.year_to_amount,
                         "year and deposit/withdrawal");
}

void ConcreteNetworthProjector::getInputFromCfg(InputDataContainer& input_data,
                                               const InputDataContainer& conf_input) {
    input_data.networth_projector = conf_input.networth_projector;
}

void ConcreteMortgageCalculator::fillSpecifier(InputDataContainer& input_data) {
    input_data.specifier = InputDataContainer::Specifier::MORTGAGE_INPUT;
}

void ConcreteMortgageCalculator::getInputFromUser(InputDataContainer& input_data) {
    getGenericInputParam(input_data.mortgage_calculator.price,
                         std::string("unit's original price"));
    getGenericInputParam(input_data.mortgage_calculator.num_months,
                         std::string("number of months"));
    getGenericInputParam(input_data.mortgage_calculator.interest_rate,
                         std::string("interest rate"));
    getGenericInputParam(input_data.mortgage_calculator.market_increase,
                         std::string("housing market inflation"));
    getGenericInputParam(input_data.mortgage_calculator.rent_to_compare,
                         std::string("rent to compare (per month)"));
    getGenericInputParam(input_data.mortgage_calculator.makelaar_fees,
                         std::string("initial expenses"));
    getGenericInputParam(input_data.mortgage_calculator.rent_annu_increase,
                         std::string("rent annual increase"));
}

void ConcreteMortgageCalculator::getInputFromCfg(InputDataContainer& input_data,
                                                 const InputDataContainer& conf_input) {
    input_data.mortgage_calculator = conf_input.mortgage_calculator;
}

void CreatorInput::getDataFromUser(InputDataContainer& input_data,
                                   const InputDataContainer& conf_input) const {
    Input* input = this->FactoryMethod();
    std::string usr_selection;
    if(getStaticUserSelectionFromMenu("data source",
                                      m_data_source_menu_displayable_choices,
                                      usr_selection)) {
        input->fillSpecifier(input_data);
        if (usr_selection == "1") {
            displayManualInputGuide();
            input->getInputFromUser(input_data);
        }
        else if (usr_selection == "2") {
            input->getInputFromCfg(input_data, conf_input);
        }
    }
}

static void getUserInputData(const CreatorInput& input,
                             InputDataContainer& input_data,
                             const InputDataContainer& conf_input) {
    input.getDataFromUser(input_data, conf_input);
}

void getProgramSelector(InputDataContainer& input_data,
                        const InputDataContainer& conf_input) {
    std::string usr_selection;
    auto selections = createChoicesMap(std::vector<std::string> {"n", "m"},
                            std::vector<std::string> {"networth projection",
                                                        "mortgage calculation"});
    if (getStaticUserSelectionFromMenu("program selector",
                                       selections,
                                       usr_selection)) {
        if (usr_selection == "n") {
            CreatorInput* creator_input = new ConcreteCreatorNetworthProjector();
            getUserInputData(*creator_input, input_data, conf_input);
        }
        else if (usr_selection == "m") {
            CreatorInput* creator_input = new ConcreteCreatorMortgageCalculator();
            getUserInputData(*creator_input, input_data, conf_input);
        }
    }
}

bool doesUserWantStaticProgram() {
    std::string usr_select;
    auto selections = createChoicesMap(std::vector<std::string> {"y", "n"},
                            std::vector<std::string> {"yes", "no"});
    if (getStaticUserSelectionFromMenu("program mode selector",
                                       selections, usr_select)) {
        if (usr_select == "y") {

        }
        else if (usr_select == "n") {

        }
    }
    return false;
}