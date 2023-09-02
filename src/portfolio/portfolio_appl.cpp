#include <iostream>
#include <fstream>
#include <sstream>
#include "cmd_common.hpp"
#include "file_generator.hpp"
#include "portfolio.hpp"
#include "portfolio_appl.hpp"
#include "portfolio_logger.hpp"
#include "portfolio_db.hpp"

namespace portfolio {

static int16_t selectPortfolio(const portfolio::PortfolioManager& portfolio_manager);

} // namespace portfolio

void portfolio::displayPortfolio(const Portfolio& obj) {
    std::cout << "Portfolio: " << obj.m_name << std::endl;
    for (const auto& investment : obj.m_investments) {
        std::cout << "Name: " << investment.getName() << ", Ticker: "
                  << investment.getTicker() << ", Quantity: "
                  << investment.getQuantity() << ", Purchase price: "
                  << investment.getPurchasePrice()
                  << std::endl;
    }
}

static int16_t portfolio::selectPortfolio(const portfolio::PortfolioManager& portfolio_manager) {
    uint16_t choice = 0;
    std::cout << "There are " << portfolio_manager.getNumPortfolios() << " portfolios" << std::endl;
    if (portfolio_manager.getNumPortfolios() > 0) {
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
    }
    return choice - 1;
}

void portfolio::executePortfolioManagement(portfolio::Portfolio& portfolio) {
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
                displayPortfolio(portfolio);
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

void portfolio::executeMultiPortfolioManagement(portfolio::PortfolioManager& portfolio_mngr) {
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
                if (portfolio_idx >= 0) {
                    portfolio::Portfolio& portfolio = portfolio_mngr.getPortfolio(portfolio_idx);
                    executePortfolioManagement(portfolio);
                }
                else {
                    std::cout << "No portfolios to select from.\n";
                }
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

portfolio::PortfolioCfgInputSource portfolio::setPortfolioInputSource() {
    PortfolioCfgInputSource selection;
    std::string usr_selection;
    auto selections = createChoicesMap(std::vector<std::string> {"d", "m", "a", "w", "f"},
                            std::vector<std::string> {"default",
                                                      "manual",
                                                      "application ui",
                                                      "web ui",
                                                      "configurations file"});
    if (getStaticUserSelectionFromMenu("portfolio configurations source",
                                       selections,
                                       usr_selection)) {
        if (usr_selection == "d") {
            selection = PortfolioCfgInputSource::DEFAULT;
        }
        else if (usr_selection == "m") {
            selection = PortfolioCfgInputSource::MANUAL_CMD;
        }
        else if (usr_selection == "a") {
            selection = PortfolioCfgInputSource::APP_UI;
        }
        else if (usr_selection == "w") {
            selection = PortfolioCfgInputSource::WEB_UI;
        }
        else if (usr_selection == "f") {
            selection = PortfolioCfgInputSource::CONF_FILE;
        }
        else {
            selection = PortfolioCfgInputSource::NONE;
        }
    }
    return selection;
}

void portfolio::setUpPortfolioManually(PortfolioMgrCfg& conf) {
    getGenericInputParam(conf.portfolio_src, std::string("portfolios source"));
    getGenericInputParam(conf.load_all_portfolios,
                            std::string("load all portfolios"));
    if (!conf.load_all_portfolios) {
        getGenericInputParam(conf.portfolio_list,
                            std::string("portfolios names"));
    }
    getGenericInputParam(conf.auto_save,
                         std::string("auto save"));
    getGenericInputParam(conf.auto_log,
                         std::string("auto log"));
}

bool portfolio::setUpPortfolioCfg(PortfolioMgrCfg& conf) {
    auto selection = setPortfolioInputSource();
    if (PortfolioCfgInputSource::MANUAL_CMD == selection) {
        setUpPortfolioManually(conf);
    }
    else if (PortfolioCfgInputSource::APP_UI == selection) {
        std::cout << "Feature not implemented yet!" << std::endl;
    }
    else if (PortfolioCfgInputSource::WEB_UI == selection) {
        std::cout << "Feature not implemented yet!" << std::endl;
    }
    else if (PortfolioCfgInputSource::CONF_FILE == selection) {
        std::cout << "Feature not implemented yet!" << std::endl;
    }
    else if (PortfolioCfgInputSource::NONE == selection) {
        std::cout << "Failed to set up!" << std::endl;
        return false;
    }
    return true;
}