#include <string>
#include <iostream>
#include "input_handler.hpp"

template <typename T>
static void getInputParam(T& input, T defVal, std::string dispTxt) {
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

InputNwProjection useUserInput() {
    InputNwProjection user_input;
    InputNwProjection defaults = fillPredefinedInput();
    getInputParam(user_input.init_nw, defaults.init_nw, std::string("initial networth"));
    getInputParam(user_input.year_income, defaults.year_income, std::string("yearly income"));
    getInputParam(user_input.age_retirement, defaults.age_retirement, std::string("age of retirement"));
    getInputParam(user_input.current_age, defaults.current_age, std::string("current age"));
    getInputParam(user_input.year_increase, defaults.year_increase, std::string("yearly increase %"));
    getInputParam(user_input.inv_yearly, defaults.inv_yearly, std::string("yearly investing %"));
    getInputParam(user_input.port_yearly_ret, defaults.port_yearly_ret, std::string("portfolio return %"));
    getInputParam(user_input.port_fees, defaults.port_fees, std::string("portfolio fees %"));
    return user_input;
}

InputNwProjection fillPredefinedInput() {
    InputNwProjection user_input;
    user_input.init_nw = 10000;
    user_input.year_income = 49000;
    user_input.age_retirement = 62;
    user_input.current_age = 22;
    user_input.year_increase = 2;
    user_input.port_yearly_ret = 10;
    user_input.port_fees = 2;
    user_input.inv_yearly = 15;
    return user_input;
}

InputNwProjection getUserInput() {
    InputNwProjection user_input;
    std::string input;
    std::cout << "Do you want to fill your own data? (y/n)";
    std::cin >> input;
    if (input == "y" || input == "Y"
        || input == "yes" || input == "YES"
        || input == "Yes") {
        std::cin.ignore( 1000000, '\n' );
        user_input = useUserInput();
    }
    else if (input == "n" || input == "N"
            || input == "no" || input == "NO"
            || input == "No") {
        std::cin.ignore( 1000000, '\n' );
        user_input = fillPredefinedInput();
    }
    else {
        std::cout << "Input must be yes or no!\n";
    }
    return user_input;
}