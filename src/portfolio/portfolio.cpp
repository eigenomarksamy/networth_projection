#include <iostream>
#include "portfolio.hpp"
#include "input_handler.hpp"

bool Portfolio::addInvestment(const Investment& investment) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == investment.getTicker()) {
            if (it->getName() != investment.getName()) {
                return retVal;
            }
            it->setPurchasePrice(investment.getPurchasePrice());
            it->setQuantity(investment.getQuantity());
            retVal = true;
            break;
        }
    }
    if (!retVal) {
        m_investments.push_back(investment);
        retVal = true;
    }
    return retVal;
}

bool Portfolio::removeInvestment(const std::string& ticker) {
    bool retVal = false;
    for (auto it = m_investments.begin(); it != m_investments.end(); ++it) {
        if (it->getTicker() == ticker) {
            m_investments.erase(it);
            retVal = true;
            break;
        }
    }
    return retVal;
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

bool Portfolio::updatedInvestmentValue(const std::string& ticker, double_t newValue) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setPurchasePrice(newValue);
            retVal = true;
            break;
        }
    }
    return retVal;
}

bool Portfolio::updateInvestmentQuantity(const std::string& ticker, uint32_t newQuant) {
    bool retVal = false;
    for (auto& investment : m_investments) {
        if (investment.getTicker() == ticker) {
            investment.setQuantity(newQuant);
            retVal = true;
            break;
        }
    }
    return retVal;
}

void PortfolioManager::executeManagement() {
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
                if (m_portfolio->addInvestment(newInvestment)) {
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
                if (m_portfolio->removeInvestment(ticker)) {
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
                if (!validateInputType(newValue)) {
                    continue;
                }
                if(m_portfolio->updatedInvestmentValue(ticker, newValue)) {
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
                if(m_portfolio->updateInvestmentQuantity(ticker, newQantity)) {
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