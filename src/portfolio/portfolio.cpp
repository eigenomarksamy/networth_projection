#include <iostream>
#include "portfolio.hpp"

void Portfolio::addInvestment(const Investment& investment) {
    m_investments.push_back(investment);
}

void Portfolio::removeInvestment(const std::string& ticker) {
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == ticker) {
            m_investments.erase(it);
            break;
        }
    }
}

void Portfolio::displayPortfolio() const {
    std::cout << "Portfolio: " << m_name << std::endl;
    for (const auto& investment : m_investments) {
        std::cout << "Name: " << investment.getName() << ", Ticker: "
                  << investment.getTicker() << ", Quantity: "
                  << investment.getQuantity() << ", Purchase price: "
                  << investment.getPurchasePrice()
                  << std::endl;
    }
}

double_t Portfolio::calculateTotalValue() const {
    double_t totalValue = 0.0;
    for (const auto& investment : m_investments) {
        totalValue += investment.getPurchasePrice() * investment.getQuantity();
    }
    return totalValue;
}

void Portfolio::updatedInvestmentValue(const std::string& ticker, double_t newValue) {
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setPurchasePrice(newValue);
            break;
        }
    }
}

void PortfolioManager::executeManagement() {
    int32_t choice = 0;
    while (choice != 6) {
        std::cout << "---------------------------" << std::endl;
        std::cout << "         MENU              " << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "1. Add Investment" << std::endl;
        std::cout << "2. Remove Investment" << std::endl;
        std::cout << "3. Display Portfolio" << std::endl;
        std::cout << "4. Calculate Total Value" << std::endl;
        std::cout << "5. Update Investment Value" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter your choice (1-6): ";
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                if (!(std::cin >> purchasePrice)) {
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cout << "Quantity: ";
                if (!(std::cin >> quantity)) {
                    std::cout << "Invalid input. Please enter an integer." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                Investment newInvestment(name, ticker, purchasePrice, quantity);
                m_portfolio->addInvestment(newInvestment);
                std::cout << "Investment of '" << newInvestment.getTicker()
                          << "' was added successfully.\n";
                break;
            }
            case 2: {
                std::string ticker;
                std::cout << "Enter the ticker of the investment to be removed: ";
                std::cin >> ticker;
                m_portfolio->removeInvestment(ticker);
                std::cout << "Investment removed from portfolio.\n";
                break;
            }
            case 3: {
                m_portfolio->displayPortfolio();
                break;
            }
            case 4: {
                auto totalValue = m_portfolio->calculateTotalValue();
                std::cout << "Portfolio's total value: " << totalValue << std::endl;
                break;
            }
            case 5: {
                std::string ticker;
                double_t newValue;
                std::cout << "Enter the ticker of the investment to be updated: ";
                std::cin >> ticker;
                std::cout << "Enter the new value: ";
                if (!(std::cin >> newValue)) {
                    std::cout << "Invalid input. Please enter a number." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                m_portfolio->updatedInvestmentValue(ticker, newValue);
                std::cout << "Investment value was updated.\n";
                break;
            }
            case 6: {
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