#ifndef ACITVATION_HPP_
#define ACTIVATION_HPP_

#include "computation_adapter.hpp"
#include "str_adapter.hpp"

void executeStaticComputation(StaticComputation& computationObj,
                              StrStreamGenerator& generatorObj,
                              const std::string& confName);

#endif /* ACTIVATION_HPP_ */