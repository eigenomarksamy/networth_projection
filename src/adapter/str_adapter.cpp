#include "str_adapter.hpp"

StrStreamGenerator::NamesWithLines StrStreamGenerator::generateNamesWithLines() const {
    NamesWithLines retObj;
    retObj.names = this->generateDataNames();
    retObj.lines = this->generateDataLines();
    return retObj;
}