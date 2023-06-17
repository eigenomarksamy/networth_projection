#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include <math.h>
#include <stdint.h>
#include <vector>
#include <iostream>

template<typename T>
bool validateInputType(T& input) {
    bool retVal = true;
    if (!(std::cin >> input)) {
        std::cout << "Invalid input. Please correct type." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        retVal = false;
    }
    return retVal;
}

struct InputDataNetworthProjector {
    uint32_t init_nw;
    uint32_t year_income;
    uint32_t age_retirement;
    uint32_t current_age;
    float_t year_increase;
    float_t port_yearly_ret;
    float_t port_fees;
    float_t inv_yearly;
    std::vector<std::vector<float_t>> year_to_amount;
};

struct InputDataMortgageCalculator {
    uint32_t price;
    uint16_t num_months;
    float_t interest_rate;
    float_t market_increase;
    float_t rent_to_compare;
    uint16_t makelaar_fees;
    float_t rent_annu_increase;
};

struct InputPortfolioManager {
    bool is_new;
    std::string name;
};

struct InputDataContainer {
    enum Specifier {
        NONE = 0,
        NETWORTH_INPUT,
        MORTGAGE_INPUT,
        PORTFOLIO_INPUT
    };
    InputDataNetworthProjector networth_projector;
    InputDataMortgageCalculator mortgage_calculator;
    InputPortfolioManager portfolio_manager;
    Specifier specifier;
};

class Input {

public:
    virtual ~Input() {}
    virtual void getInputFromUser(InputDataContainer& input_data) = 0;
    virtual void getInputFromCfg(InputDataContainer& input_data) = 0;
    virtual void getInputFromDefaults(InputDataContainer& input_data) = 0;
    virtual void fillDefaults() = 0;
};

class ConcreteNetworthProjector : public Input {
    uint32_t m_init_nw;
    uint32_t m_year_income;
    uint32_t m_age_retirement;
    uint32_t m_current_age;
    float_t m_year_increase;
    float_t m_port_yearly_ret;
    float_t m_port_fees;
    float_t m_inv_yearly;
    std::vector<std::vector<float_t>> m_year_to_amount;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void getInputFromDefaults(InputDataContainer& input_data) override;
    void fillDefaults() override;
};

class ConcreteMortgageCalculator : public Input {

    uint32_t m_price;
    uint16_t m_num_months;
    float_t m_interest_rate;
    float_t m_market_increase;
    float_t m_rent_to_compare;
    uint16_t m_makelaar_fees;
    float_t m_rent_annu_increase;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void getInputFromDefaults(InputDataContainer& input_data) override;
    void fillDefaults() override;
};

class ConcretePortfolioManager : public Input {
    bool m_is_new;
    std::string m_name;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void getInputFromDefaults(InputDataContainer& input_data) override;
    void fillDefaults() override;
};

class CreatorInput {

public:

virtual ~CreatorInput() {}
virtual Input* FactoryMethod() const = 0;

void getDataFromUser(InputDataContainer& input_data) const;

};

class ConcreteCreatorNetworthProjector : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcreteNetworthProjector();
    }
};

class ConcreteCreatorMortgageCalculator : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcreteMortgageCalculator();
    }
};

class ConcreteCreatorPortfolioManager : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcretePortfolioManager();
    }
};

void getUserSelection(InputDataContainer& input_data);

#endif /* INPUT_HANDLER_HPP_ */