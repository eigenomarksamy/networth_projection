#include <iostream>
#include "continuous_input.hpp"
#include "input_handler.hpp"

int16_t selectPortfolio(PortfolioManager& portfolio_manager) {
    uint16_t choice = 0;
    std::cout << "There are " << portfolio_manager.getNumPortfolios() << " portfolios" << std::endl;
    std::cout << "Please select from this list:\n";
    for (auto i = 0; i < portfolio_manager.getNumPortfolios(); ++i) {
        std::cout << "Portfolio ID: " << i + 1
                  << ", Name: " << portfolio_manager.getPortfolio(i).getName()
                  << std::endl;
    }
    do {
        std::cout << "Enter your choice (1-"
                << portfolio_manager.getNumPortfolios()
                << "): ";
    }
    while (!validateInputRange(choice, (uint16_t)0, portfolio_manager.getNumPortfolios()));
    return choice - 1;
}

void executePortfolioManagement(Portfolio& portfolio) {
    int32_t choice = 0;
    while (choice != 7) {
        std::cout << "---------------------------" << std::endl;
        std::cout << "         MENU              " << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "1. Add Investment" << std::endl;
        std::cout << "2. Remove Investment" << std::endl;
        std::cout << "3. Display Portfolio" << std::endl;
        std::cout << "4. Calculate Total Value" << std::endl;
        std::cout << "5. Update Investment Value" << std::endl;
        std::cout << "6. Update Investment Quantity" << std::endl;
        std::cout << "7. Exit" << std::endl;
        std::cout << "Enter your choice (1-7): ";
        if (!validateInputType(choice)) {
            continue;
        }
        switch (choice) {
            case 1: {
                std::string name, ticker;
                double_t purchasePrice;
                uint32_t quantity;
                std::cout << "Enter investment details:\n";
                std::cout << "Name: ";
                std::cin >> name;
                std::cout << "Ticker: ";
                std::cin >> ticker;
                std::cout << "Purchase price: ";
                if (!validateInputType(purchasePrice)) {
                    continue;
                }
                std::cout << "Quantity: ";
                if (!validateInputType(quantity)) {
                    continue;
                }
                Investment newInvestment(name, ticker, purchasePrice, quantity);
                if (portfolio.addInvestment(newInvestment)) {
                    std::cout << "Investment of '" << newInvestment.getTicker()
                              << "' was added successfully.\n";
                }
                else {
                    std::cout << "Investment of '" << newInvestment.getTicker()
                              << "' already exists, failed to update; "
                              << "it's stored with a different name.\n"
                              << "hint: remove investment and add it again.\n";
                    continue;
                }
                break;
            }
            case 2: {
                std::string ticker;
                std::cout << "Enter the ticker of the investment to be removed: ";
                std::cin >> ticker;
                if (portfolio.removeInvestment(ticker)) {
                    std::cout << "Investment removed from portfolio.\n";
                }
                else {
                    std::cout << "Investment not found, "
                              << "can't remove what's not there.\n";
                    continue;
                }
                break;
            }
            case 3: {
                portfolio.displayPortfolio();
                break;
            }
            case 4: {
                auto totalValue = portfolio.calculateTotalValue();
                std::cout << "Portfolio's total value: " << totalValue << std::endl;
                break;
            }
            case 5: {
                std::string ticker;
                double_t newValue;
                std::cout << "Enter the ticker of the investment to be updated: ";
                std::cin >> ticker;
                std::cout << "Enter the new value: ";
                if (!validateInputType(newValue)) {
                    continue;
                }
                if(portfolio.updatedInvestmentValue(ticker, newValue)) {
                    std::cout << "Investment value was updated.\n";
                }
                else {
                    std::cout << "Investment not found!\n";
                }
                break;
            }
            case 6: {
                std::string ticker;
                uint32_t newQantity;
                std::cout << "Enter the ticker of the investment to be updated: ";
                std::cin >> ticker;
                std::cout << "Enter the new quantity: ";
                if (!validateInputType(newQantity)) {
                    continue;
                }
                if(portfolio.updateInvestmentQuantity(ticker, newQantity)) {
                    std::cout << "Investment value was updated.\n";
                }
                else {
                    std::cout << "Investment not found!\n";
                }
                break;
            }
            case 7: {
                std::cout << "Exiting..\n";
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}