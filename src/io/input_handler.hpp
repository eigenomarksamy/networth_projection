#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_


struct Input {
    int init_nw;
    int year_income;
    int age_retirement;
    int current_age;
    float year_increase;
    float port_yearly_ret;
    float port_fees;
    float perc_inv;
};

Input useUserInput();
Input fillPredefinedInput();
Input getUserInput();

#endif /* INPUT_HANDLER_HPP_ */