#ifndef INPUT_FACTORY_HPP_
#define INPUT_FACTORY_HPP_

#include <map>
#include "input_types_cfg.hpp"
#include "cmd_common.hpp"
#include "portfolio.hpp"

class Input {

public:
    virtual ~Input() {}
    virtual void getInputFromUser(InputDataContainer& input_data) = 0;
    virtual void getInputFromCfg(InputDataContainer& input_data) = 0;
    virtual void fillSpecifier(InputDataContainer& input_data) = 0;
};

class ConcreteNetworthProjector : public Input {
    InputDataNetworthProjector m_params;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void fillSpecifier(InputDataContainer& input_data) override;
};

class ConcreteMortgageCalculator : public Input {
    InputDataMortgageCalculator m_params;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void fillSpecifier(InputDataContainer& input_data) override;
};

class ConcretePortfolioManager : public Input {
    InputPortfolioManager m_params;

public:
    void getInputFromUser(InputDataContainer& input_data) override;
    void getInputFromCfg(InputDataContainer& input_data) override;
    void fillSpecifier(InputDataContainer& input_data) override;
};

class CreatorInput {
    std::map<std::string, std::string> m_data_source_menu_displayable_choices;
public:
    CreatorInput() {
        m_data_source_menu_displayable_choices = createChoicesMap(
            std::vector<std::string> {"1", "2", "3"},
            std::vector<std::string> {
                "Manually entering your data.",
                "Fill your data in configurations file.",
                "Use predefined data."
            });
    }
    virtual ~CreatorInput() {}
    virtual Input* FactoryMethod() const = 0;
    void getDataFromUser(InputDataContainer& input_data) const;
};

class ConcreteCreatorNetworthProjector : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcreteNetworthProjector();
    }
};

class ConcreteCreatorMortgageCalculator : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcreteMortgageCalculator();
    }
};

class ConcreteCreatorPortfolioManager : public CreatorInput {
public:
    Input* FactoryMethod() const override {
        return new ConcretePortfolioManager();
    }
};

void getProgramSelector(InputDataContainer& input_data);

void executePortfolioManagement(portfolio::Portfolio& portfolio);

void executeMultiPortfolioManagement(portfolio::PortfolioManager& portfolio_mngr);

#endif /* INPUT_FACTORY_HPP_ */