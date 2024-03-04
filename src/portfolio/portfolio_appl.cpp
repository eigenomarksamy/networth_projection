#include <iostream>
#include <fstream>
#include <sstream>
#include "datetime.hpp"
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
                  << investment.getPurchasePrice() << ", Current price: "
                  << investment.getCurrentPrice()
                  << std::endl;
    }
}

static void displayComplexInvestment(const ComplexInvestment& investment) {
    auto date_str = std::to_string(investment.getTransaction().m_datetime.getTime().m_hour) + ":"
                    + std::to_string(investment.getTransaction().m_datetime.getTime().m_minute) + ":"
                    + std::to_string(investment.getTransaction().m_datetime.getTime().m_second) + " - "
                    + std::to_string(investment.getTransaction().m_datetime.getDate().m_day) + "/"
                    + std::to_string(investment.getTransaction().m_datetime.getDate().m_month) + "/"
                    + std::to_string(investment.getTransaction().m_datetime.getDate().m_year);
    auto currency_str = (investment.getTransaction().m_currency == Transaction::Currency::EUR) ? "EUR" : "USD";
    std::cout << "ID: " << investment.getId()
                << ", Sequence: " << investment.getSequencer()
                << ", Date (hh:mm:ss - dd/mm/yyyy): " << date_str
                << ", Name: " << investment.getInvestment().getName()
                << ", Ticker: " << investment.getInvestment().getTicker()
                << ", Quantity: " << investment.getInvestment().getQuantity()
                << ", Purchase price: " << investment.getInvestment().getPurchasePrice()
                << ", Current price: " << investment.getInvestment().getCurrentPrice()
                << ", Currency: " << currency_str
                << ", Fees: " << investment.getTransaction().m_fees
                << ", Currency Conversion Fees: " << investment.getTransaction().m_conversion_fees
                << ", Currency Conversion Rate: " << investment.getCurrencyConversionRate()
                << std::endl;
}

void portfolio::displayPortfolio(const TransactionalPortfolio& obj) {
    std::cout << "Portfolio: " << obj.m_name << std::endl;
    for (const auto& investment : obj.m_investments) {
        auto date_str = std::to_string(investment.getTransaction().m_datetime.getTime().m_hour) + ":"
                        + std::to_string(investment.getTransaction().m_datetime.getTime().m_minute) + ":"
                        + std::to_string(investment.getTransaction().m_datetime.getTime().m_second) + " - "
                        + std::to_string(investment.getTransaction().m_datetime.getDate().m_day) + "/"
                        + std::to_string(investment.getTransaction().m_datetime.getDate().m_month) + "/"
                        + std::to_string(investment.getTransaction().m_datetime.getDate().m_year);
        auto currency_str = (investment.getTransaction().m_currency == Transaction::Currency::EUR) ? "EUR" : "USD";
        std::cout << "ID: " << investment.getId()
                  << ", Sequence: " << investment.getSequencer()
                  << ", Date (hh:mm:ss - dd/mm/yyyy): " << date_str
                  << ", Name: " << investment.getInvestment().getName()
                  << ", Ticker: " << investment.getInvestment().getTicker()
                  << ", Quantity: " << investment.getInvestment().getQuantity()
                  << ", Purchase price: " << investment.getInvestment().getPurchasePrice()
                  << ", Current price: " << investment.getInvestment().getCurrentPrice()
                  << ", Currency: " << currency_str
                  << ", Fees: " << investment.getTransaction().m_fees
                  << ", Currency Conversion Fees: " << investment.getTransaction().m_conversion_fees
                  << ", Currency Conversion Rate: " << investment.getCurrencyConversionRate()
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

void portfolio::executePortfolioManagement(Portfolio& portfolio) {
    int32_t choice = 0;
    while (choice != 8) {
        std::cout << "---------------------------" << std::endl;
        std::cout << "         MENU              " << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "1. Add Investment" << std::endl;
        std::cout << "2. Remove Investment" << std::endl;
        std::cout << "3. Display Portfolio" << std::endl;
        std::cout << "4. Calculate Total Value" << std::endl;
        std::cout << "5. Calculate Total Gain" << std::endl;
        std::cout << "6. Update Investment Value" << std::endl;
        std::cout << "7. Update Investment Quantity" << std::endl;
        std::cout << "8. Return to portfolio menu" << std::endl;
        std::cout << "Enter your choice (1-8): ";
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
                Investment newInvestment(name, ticker, purchasePrice, purchasePrice, quantity);
                std::cout << "Validation Complete!\n";
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
                auto totalGain = portfolio.calculateTotalGain();
                std::cout << "Portfolio's total gain: " << totalGain << std::endl;
                break;
            }
            case 6: {
                std::string ticker;
                double_t newValue;
                std::cout << "Enter the ticker of the investment to be updated: ";
                std::cin >> ticker;
                std::cout << "Enter the new value: ";
                if (!validateInputType(newValue)) {
                    continue;
                }
                std::cout << "Validation Complete!\n";
                if(portfolio.updatedInvestmentValue(ticker, newValue)) {
                    std::cout << "Investment value was updated.\n";
                }
                else {
                    std::cout << "Investment not found!\n";
                }
                break;
            }
            case 7: {
                std::string ticker;
                uint32_t newQuantity;
                std::cout << "Enter the ticker of the investment to be updated: ";
                std::cin >> ticker;
                std::cout << "Enter the new quantity: ";
                if (!validateInputType(newQuantity)) {
                    continue;
                }
                std::cout << "Validation Complete!\n";
                if(portfolio.updateInvestmentQuantity(ticker, newQuantity)) {
                    std::cout << "Investment value was updated.\n";
                }
                else {
                    std::cout << "Investment not found!\n";
                }
                break;
            }
            case 8: {
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

void portfolio::executeMultiPortfolioManagement(PortfolioManager& portfolio_mngr) {
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

void portfolio::executeTransactionalPortfolioManagement(TransactionalPortfolio& portfolio) {
    int32_t choice = INT32_MAX;
    while (choice != 0) {
        std::cout << "---------------------------" << std::endl;
        std::cout << "         MENU              " << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "1. Add Investment" << std::endl;
        std::cout << "2. Remove Investment" << std::endl;
        std::cout << "11. Display Portfolio" << std::endl;
        std::cout << "12. Display Transactions of Symbols" << std::endl;
        std::cout << "13. Display Transactions of Dates" << std::endl;
        std::cout << "21. Calculate Total Value" << std::endl;
        std::cout << "22. Calculate Total Gain" << std::endl;
        std::cout << "23. Calculate Total Spending" << std::endl;
        std::cout << "24. Calculate Value of Symbols" << std::endl;
        std::cout << "25. Calculate Gain of Symbols" << std::endl;
        std::cout << "26. Calculate Spending of Symbols" << std::endl;
        std::cout << "27. Calculate Value of Date" << std::endl;
        std::cout << "28. Calculate Gain of Date" << std::endl;
        std::cout << "29. Calculate Spending of Date" << std::endl;
        std::cout << "31. Update Investment Purchase Value" << std::endl;
        std::cout << "32. Update Investment Current Value" << std::endl;
        std::cout << "33. Update Investment Quantity" << std::endl;
        std::cout << "0. Return to portfolio menu" << std::endl;
        std::cout << "Enter your choice: ";
        if (!validateInputType(choice)) {
            continue;
        }
        switch (choice) {
            case 0: {
                std::cout << "Returning..\n";
                break;
            }
            case 1: {
                std::cout << "Enter investment details:\n";
                std::string name, ticker;
                std::cout << "Name: ";
                std::cin >> name;
                std::cout << "Ticker: ";
                std::cin >> ticker;
                double_t purchasePrice;
                std::cout << "Purchase price: ";
                if (!validateInputType(purchasePrice)) {
                    continue;
                }
                uint32_t quantity;
                std::cout << "Quantity: ";
                if (!validateInputType(quantity)) {
                    continue;
                }
                Investment investment(name, ticker, purchasePrice, purchasePrice, quantity);
                double_t fees;
                std::cout << "Fees: ";
                if (!validateInputType(fees)) {
                    continue;
                }
                DateTime date;
                date.setToNow();
                // std::cout << "Date (leave blank to use now):\n";
                // getGenericInputParam(date.m_hour, std::string("hour"));
                // getGenericInputParam(date.m_day, std::string("day"));
                // getGenericInputParam(date.m_month, std::string("month"));
                // getGenericInputParam(date.m_year, std::string("year"));
                std::string currency;
                std::cout << "Currency (USD or EUR): ";
                std::cin >> currency;
                convertStrToLowerCase(currency);
                Transaction::Currency tCurrency = (currency == "usd") ? Transaction::Currency::USD : Transaction::Currency::EUR;
                uint32_t sequencer = portfolio.getInvestments().size();
                double_t currency_conv_fees;
                std::cout << "Currency conversion fees: ";
                if (!validateInputType(currency_conv_fees)) {
                    continue;
                }
                double_t currency_conv_rate;
                std::cout << "Currency conversion rate: ";
                if (!validateInputType(currency_conv_rate)) {
                    continue;
                }
                auto complex_investment = TransactionalPortfolio::createComplexInvestment(investment,
                                            date, fees, tCurrency, sequencer,
                                            currency_conv_rate, currency_conv_fees);
                if (portfolio.addInvestments({complex_investment})) {
                    std::cout << "Investment was added successfully.\n";
                }
                else {
                    std::cout << "Investment already exists, failed to add\n";
                    continue;
                }
                break;
            }
            case 2: {
                std::string id;
                std::cout << "Enter the id of the investment to be removed: ";
                std::cin >> id;
                if (portfolio.removeInvestments({id})) {
                    std::cout << "Investment removed from portfolio.\n";
                    portfolio.updateSequence();
                }
                else {
                    std::cout << "Investment not found, "
                              << "can't remove what's not there.\n";
                    continue;
                }
                break;
            }
            case 11: {
                displayPortfolio(portfolio);
                break;
            }
            case 12: {
                std::string id;
                std::cout << "Enter the ticker of the symbol to be displayed: ";
                std::cin >> id;
                auto investments = portfolio.getFilteredSymbolInvestments(id);
                for (const auto& investment : investments) {
                    displayComplexInvestment(investment);
                }
                break;
            }
            case 13: {
                // std::string day, month, year;
                // std::string raw_date;
                // std::cout << "Enter the date to be displayed (dd/mm/yyyy): ";
                // std::cin >> raw_date;
                // std::vector<std::string> split;
                // splitStr(raw_date, '/', split);
                // Date date (0, 0, 0, 0, 0, 0);
                // TransactionalPortfolio::DateFilterAttributes attribute;
                // if (split.size() == 3) {
                //     attribute = TransactionalPortfolio::DateFilterAttributes::Day;
                //     date.m_day = std::stoi(split[0]);
                //     date.m_month = std::stoi(split[1]);
                //     date.m_year = std::stoi(split[2]);
                // }
                // if (split.size() == 2) {
                //     attribute = TransactionalPortfolio::DateFilterAttributes::Month;
                //     date.m_month = std::stoi(split[0]);
                //     date.m_year = std::stoi(split[1]);
                // }
                // if (split.size() == 1) {
                //     attribute = TransactionalPortfolio::DateFilterAttributes::Year;
                //     date.m_year = std::stoi(split[0]);
                // }
                // // if (!date.validate()) {
                // //     continue;
                // // }
                // auto investments = portfolio.getFilteredDateInvestments(date, attribute);
                // for (const auto& investment : investments) {
                //     displayComplexInvestment(investment);
                // }
                break;
            }
            case 21: {
                break;
            }
            case 22: {
                break;
            }
            case 23: {
                break;
            }
            case 24: {
                break;
            }
            case 25: {
                break;
            }
            case 26: {
                break;
            }
            case 27: {
                break;
            }
            case 28: {
                break;
            }
            case 29: {
                break;
            }
            case 31: {
                break;
            }
            case 32: {
                break;
            }
            case 33: {
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

void portfolio::setUpPortfolioConfigurationFile(PortfolioMgrCfg& conf, const std::string& dirFile) {
    PortfolioMgrYmlDirs yml_cfg_dirs;
    if (resolveCfg(dirFile, yml_cfg_dirs)) {
        convertYmlData(conf, yml_cfg_dirs);
    }
}

bool portfolio::setUpPortfolioCfg(PortfolioMgrCfg& conf, const std::string& dirFile) {
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
        setUpPortfolioConfigurationFile(conf, dirFile);
    }
    else if (PortfolioCfgInputSource::NONE == selection) {
        std::cout << "Failed to set up!" << std::endl;
        return false;
    }
    return true;
}