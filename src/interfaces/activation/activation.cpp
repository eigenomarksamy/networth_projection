#include "activation.hpp"
#include "conf_input_types.hpp"
#include "input_types_cfg.hpp"
#include "file_generator.hpp"

void generateDataCsv(const StrStreamGenerator& generatorObj,
                     FileGenerator& fileObj) {
    auto genRet = generatorObj.generateNamesWithLines();
    fileObj.generateCsv(genRet.names, genRet.lines);
}

void generateInputTxt(const StrStreamGenerator& generatorObj,
                     FileGenerator& fileObj) {
    auto lines = generatorObj.generateInputPreview();
    fileObj.generateTxt(lines);
}

void executeStaticComputation(StaticComputation& computationObj,
                              const std::string& confName) {
    computationObj.computeData();
    // generatorObj.generateDataLines();
}