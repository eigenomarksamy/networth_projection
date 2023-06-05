#include <iostream>
#include <string>
#include "NetWorth.hpp"

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


// if(isdigit(input[0])){
//     myint = atoi(input.c_str());
//     cout << "Number entered";
// }else{
//     cout << "Error.....Input was not a number";
// }


    // const char newline = '\n' ;

    // std::cout << "enter an integer (or just enter to quit): " ;
    // int a ;
    // if( std::cin.peek() != newline && std::cin >> a )
    // {
    //     std::cin.ignore( 1000000, '\n' ) ; // extract and throw away the new line

    //     std::cout << "enter another integer (or just enter to quit): " ;
    //     int b ;
    //     if( std::cin.peek() != newline && std::cin >> b )
    //         std::cout << a << " + " << b  << " == " << a+b << '\n' ;
    // }


Input useUserInput() {
    Input user_input;
    std::cout << "Enter initial networth: ";
    std::cin >> user_input.init_nw;
    std::cout << "Enter yearly income: ";
    std::cin >> user_input.year_income;
    std::cout << "Enter age of retirement: ";
    std::cin >> user_input.age_retirement;
    std::cout << "Enter current age: ";
    std::cin >> user_input.current_age;
    std::cout << "Enter yearly increase % : ";
    std::cin >> user_input.year_increase;
    std::cout << "Enter yearly investing %: ";
    std::cin >> user_input.perc_inv;
    std::cout << "Enter yearly portfolio return %: ";
    std::cin >> user_input.port_yearly_ret;
    std::cout << "Enter yearly portfolio fees %: ";
    std::cin >> user_input.port_fees;
    return user_input;
}

Input fillPredefinedInput() {
    Input user_input;
    user_input.init_nw = 10000;
    user_input.year_income = 49000;
    user_input.age_retirement = 62;
    user_input.current_age = 22;
    user_input.year_increase = 2;
    user_input.port_yearly_ret = 10;
    user_input.port_fees = 2;
    user_input.perc_inv = 15;
    return user_input;
}

Input getUserInput() {
    Input user_input;
    std::string input;
    std::cout << "Do you want to fill your own data? (y/n)";
    std::cin >> input;
    if (input == "y" || input == "Y"
        || input == "yes" || input == "YES"
        || input == "Yes") {
        user_input = useUserInput();
    }
    else if (input == "n" || input == "N"
            || input == "no" || input == "NO"
            || input == "No") {
        user_input = fillPredefinedInput();
    }
    else {
        std::cout << "Input must be yes or no!\n";
    }
    return user_input;
}


int main() {
    auto user_input = getUserInput();
    NetWorth networth(user_input.init_nw, user_input.year_income,
                      user_input.age_retirement, user_input.current_age,
                      NetWorth::Percentages({user_input.year_increase,
                                             user_input.port_yearly_ret,
                                             user_input.port_fees,
                                             user_input.perc_inv}));
    networth.computeData();
    networth.printTabulatedData();
    return 0;
}