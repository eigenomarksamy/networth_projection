#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <math.h>

template<typename T>
float getPercent(T percent) {
    return static_cast<float>(percent) / 100.0;
}

template<typename T, typename R, typename U>
float computePMT(T pv, R r, U n) {
    return static_cast<float>(-((pv * (r / 12)) / (1. - pow(1. + (r / 12), -n))));
}

std::string getLocalDateTime(const char format[]);

std::string getLocalDateTime();

#endif /* UTIL_HPP_ */