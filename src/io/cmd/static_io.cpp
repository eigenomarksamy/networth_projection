#include <map>
#include <string>
#include <iostream>
#include "cmd_common.hpp"

bool getStaticUserSelectionFromMenu(const std::string& title,
                                    const std::map<std::string, std::string>& options,
                                    std::string& selection) {
    std::string raw_selection;
    bool validity = false;
    std::cout << "Welcome to " << title << " selection menu.\n";
    if (options.size() > 1) {
        std::cout << "Options:\n";
    }
    else if (options.size() > 0) {
        std::cout << "Option:\n";
    }
    for (const auto& option : options) {
        std::cout << "'" << option.first << "': " << option.second << std::endl;
    }
    if (options.size() >= 1) {
        std::cout << "Selection: ";
        std::cin >> raw_selection;
        for (const auto& option : options) {
            if (raw_selection == option.first) {
                validity = true;
                std::cin.ignore( 1000000, '\n' );
                selection = raw_selection;
                break;
            }
        }
        if (!validity)
            std::cerr << "Unknown selection!\n";
    }
    return validity;
}

void displayManualInputGuide() {
    std::cout << "Usage modes for manual input:\n";
    std::cout << "\t" << GenericInputMode::SingleValueSingleData;
    std::cout << ": Enter a single value, then new line to move on.\n";
    std::cout << "\t" << GenericInputMode::MultipleValuesSingleData;
    std::cout << ": Enter multiple single values (space seperated), "
                << "and new line to finish and move on.\n";
    std::cout << "\t" << GenericInputMode::MultipleValuesMultipleData;
    std::cout << ": Enter multiple multiple values, "
                << "comma separed for one set and "
                << "space seperated for different sets, "
                << "and new line to finish and move on.\n";
    std::cout << std::endl;
}