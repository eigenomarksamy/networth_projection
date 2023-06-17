#include <iostream>
#include <string>
#include "continuous_input.hpp"
#include "input_handler.hpp"

static int16_t selectPortfolio(const PortfolioManager& portfolio_manager) {
    uint16_t choice = 0;
    std::cout << "There are " << portfolio_manager.getNumPortfolios() << " portfolios" << std::endl;
    std::cout << "Please select portfolio from this list:\n";
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

static void executePortfolioManagement(Portfolio& portfolio) {
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
        std::cout << "7. Return to portfolio menu" << std::endl;
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
                std::cout << "Returning..\n";
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
}

void executeMultiPortfolioManagement(PortfolioManager& portfolio_mngr) {
    int32_t choice = 0;
    while (choice != 4) {
        std::cout << "---------------------------" << std::endl;
        std::cout << "         MENU              " << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "1. Create Portfolio" << std::endl;
        std::cout << "2. Delete Portfolio" << std::endl;
        std::cout << "3. Select Portfolio" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice (1-4): ";
        if (!validateInputType(choice)) {
            continue;
        }
        switch (choice) {
            case 1: {
                std::string name;
                std::cout << "Enter new portfolio name: ";
                std::cin >> name;
                if (portfolio_mngr.addPortfolio(name)) {
                    std::cout << "Portfolio created successfully.\n";
                }
                else {
                    std::cout << "Portfolio found with the same name.\n";
                }
                break;
            }
            case 2: {
                std::string name;
                std::cout << "Enter portfolio name: ";
                std::cin >> name;
                if (portfolio_mngr.removePortfolio(name)) {
                    std::cout << "Portfolio deleted successfully.\n";
                }
                else {
                    std::cout << "Portfolio couldn't be found with the name.\n";
                }
                break;
            }
            case 3: {
                auto portfolio_idx = selectPortfolio(portfolio_mngr);
                Portfolio& portfolio = portfolio_mngr.getPortfolio(portfolio_idx);
                executePortfolioManagement(portfolio);
                break;
            }
            case 4: {
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