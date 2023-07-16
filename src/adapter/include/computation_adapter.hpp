#ifndef COMPUTATION_ADAPTER_HPP_
#define COMPUTATION_ADAPTER_HPP_

#include <vector>
#include <string>

class StaticComputation {

public:
    virtual void printTabulatedData() const = 0;
    virtual void computeData() = 0;
    virtual std::vector<std::string> getDataNames() const = 0;
};

#endif /* COMPUTATION_ADAPTER_HPP_ */