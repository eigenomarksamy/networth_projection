#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_


struct InputNwProjection {
    int init_nw;
    int year_income;
    int age_retirement;
    int current_age;
    float year_increase;
    float port_yearly_ret;
    float port_fees;
    float inv_yearly;
};

InputNwProjection useUserInput();
InputNwProjection fillPredefinedInput();
InputNwProjection getUserInput();

class InputMortgageCalculator {

public:

    InputMortgageCalculator () {
        m_price = 425000;
        m_interest_rate = 4.f;
        m_num_months = 360;
    }

    InputMortgageCalculator(uint32_t price,
                            float_t interest_rate,
                            uint32_t num_months)
        : m_price(price), m_interest_rate(interest_rate),
          m_num_months(num_months) { }

private:

    uint32_t m_price;
    float_t m_interest_rate;
    uint32_t m_num_months;
};

#endif /* INPUT_HANDLER_HPP_ */