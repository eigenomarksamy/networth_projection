#ifndef CMD_COMMON_HPP_
#define CMD_COMMON_HPP_

#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <sstream>

enum class GenericInputMode { SingleValueSingleData,
                              MultipleValuesSingleData,
                              MultipleValuesMultipleData };

inline std::ostream& operator<<(std::ostream& out, const GenericInputMode& inputMode) {
    switch (inputMode) {
        case GenericInputMode::SingleValueSingleData:
            out << "Single Value Single Data";
            break;
        case GenericInputMode::MultipleValuesSingleData:
            out << "Multiple Values Single Data";
            break;
        case GenericInputMode::MultipleValuesMultipleData:
            out << "Multiple Values Multiple Data";
            break;
    }
    return out;
}

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

template<typename T>
bool validateInputRange(T& input, const T& start, const T& end) {
    bool retVal = true;
    if (!(std::cin >> input)) {
        std::cout << "Invalid input. Please correct type." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        retVal = false;
    }
    else if ((input < start) || (input > end)) {
        std::cout << "Invalid input. Please correct range." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        retVal = false;
    }
    return retVal;
}

template <typename T>
void getGenericInputParam(T& input, const T defVal, std::string dispTxt) {
    constexpr int8_t newline = '\n' ;
    std::cout << "Enter parameter '" << dispTxt << "', input mode: '"
              << GenericInputMode::SingleValueSingleData
              << "' (you can leave blank to use default): " ;
    if( std::cin.peek() != newline && std::cin >> input ) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), newline );
        return;
    }
    else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), newline );
        input = defVal;
    }
}

template <typename T>
void getGenericInputParam(T& input, std::string dispTxt) {
    getGenericInputParam(input, input, dispTxt);
}

template <typename T>
void getGenericInputParam(std::vector<T>& input,
                          const std::string& dispTxt) {
    constexpr int8_t newline = '\n' ;
    T inputElm;
    std::cout << "Enter parameter '" << dispTxt << "', input mode: '"
              << GenericInputMode::MultipleValuesSingleData
              << "' (you can leave blank to use default): " ;
    while ( std::cin.peek() != newline && std::cin >> inputElm ) {
        input.push_back(inputElm);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), newline);
}

template <typename T>
void getGenericInputParam(std::vector<std::vector<T>>& input,
                          const std::string& dispTxt) {
    constexpr int8_t newline = '\n' ;
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), newline);
}

template<typename T>
std::map<std::string, std::string> createChoicesMap(
                                                const std::vector<T>& choices,
                                                const std::vector<std::string>& descriptions) {
    std::map<T, std::string> map_out;
    if (descriptions.size() == choices.size()) {
        for (size_t i = 0; i < choices.size(); ++i) {
            map_out.insert(std::make_pair(static_cast<std::string>(choices[i]), descriptions[i]));
        }
    }
    return map_out;
}

bool getStaticUserSelectionFromMenu(const std::string& title,
                                    const std::map<std::string, std::string>& options,
                                    std::string& selection);

void displayManualInputGuide();

bool getUserYesNo(const std::string& question, const bool defVal=true);

#endif /* CMD_COMMON_HPP_ */