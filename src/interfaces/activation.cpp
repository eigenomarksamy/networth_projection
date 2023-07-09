#include "activation.hpp"
#include "computation_adapter.hpp"
#include "str_adapter.hpp"
#include "file_generator.hpp"
#include "networth.hpp"
#include "mortgage.hpp"
#include "utils.hpp"

static void generateDataCsv(const StrStreamGenerator& generatorObj,
                            FileGenerator& fileObj);
static void generateInputTxt(const StrStreamGenerator& generatorObj,
                             FileGenerator& fileObj);
static void generateStaticComputationFiles(const std::string& fileOutput,
                                           const std::string& fileInputPreview,
                                           const StrStreamGenerator& generatorObj);

static void generateDataCsv(const StrStreamGenerator& generatorObj,
                            FileGenerator& fileObj) {
    auto genRet = generatorObj.generateNamesWithLines();
    fileObj.generateCsv(genRet.names, genRet.lines);
}

static void generateInputTxt(const StrStreamGenerator& generatorObj,
                             FileGenerator& fileObj) {
    auto lines = generatorObj.generateInputPreview();
    fileObj.generateTxt(lines);
}

static void generateStaticComputationFiles(const std::string& fileOutput,
                                           const std::string& fileInputPreview,
                                           const StrStreamGenerator& generatorObj) {
    FileGenerator file_handler_output(fileOutput);
    FileGenerator file_handler(fileInputPreview);
    generateDataCsv(generatorObj, file_handler_output);
    generateInputTxt(generatorObj, file_handler);
}

void executeStaticComputation(const InputDataContainer& inputObj,
                              const DirectoryGeneratorCfg& dirCfg) {
    if (inputObj.specifier == InputDataContainer::Specifier::NETWORTH_INPUT) {
        std::unordered_map<uint32_t, float_t> extra_money_map;
        convert2DVectorToUnorderedMap(inputObj.networth_projector.year_to_amount,
                                      extra_money_map);
        auto computationObj = networth::NetWorth(
                                inputObj.networth_projector.init_nw,
                                inputObj.networth_projector.year_income,
                                inputObj.networth_projector.age_retirement,
                                inputObj.networth_projector.current_age,
                                extra_money_map,
                                networth::NetWorth::Percentages({
                                    inputObj.networth_projector.year_increase,
                                    inputObj.networth_projector.port_yearly_ret,
                                    inputObj.networth_projector.port_fees,
                                    inputObj.networth_projector.inv_yearly}));
        computationObj.computeData();
        computationObj.printTabulatedData();
        auto generationObj = networth::NetworthAdapter(computationObj);
        generateStaticComputationFiles(dirCfg.networth_projector.output.value,
                                       dirCfg.networth_projector.input.value,
                                       generationObj);
    }
    else if (inputObj.specifier == InputDataContainer::Specifier::MORTGAGE_INPUT) {
        auto computationObj = mortgage::Mortgage(
                                inputObj.mortgage_calculator.price,
                                inputObj.mortgage_calculator.num_months,
                                inputObj.mortgage_calculator.interest_rate,
                                inputObj.mortgage_calculator.market_increase,
                                inputObj.mortgage_calculator.rent_to_compare,
                                inputObj.mortgage_calculator.makelaar_fees,
                                inputObj.mortgage_calculator.rent_annu_increase);
        computationObj.computeData();
        computationObj.printTabulatedData();
        auto generationObj = mortgage::MortgageAdapter(computationObj);
        generateStaticComputationFiles(dirCfg.mortgage_calculator.output.value,
                                       dirCfg.mortgage_calculator.input.value,
                                       generationObj);
    }
}