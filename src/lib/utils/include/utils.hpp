#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <math.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>

template<typename T>
float_t getPercent(T percent) {
    return static_cast<float_t>(percent) / 100.f;
}

template<typename T, typename R, typename U>
float_t computePMT(T pv, R r, U n) {
    return static_cast<float_t>(-((pv * (r / 12)) / (1. - pow(1. + (r / 12), -n))));
}

template<typename T>
void convert2DVectorToUnorderedMap(const std::vector<std::vector<T>>& vector,
                                   std::unordered_map<uint32_t, T>& map) {
    if (vector.size() > 0) {
        if (vector[0].size() == 2) {
            for (const auto& vec : vector) {
                map.insert(std::make_pair(static_cast<uint32_t>(vec[0]), vec[1]));
            }
        }
    }
}

template<typename K, typename V>
void printMap(std::ostream& out, std::unordered_map<K, V> const &m) {
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K, V>& m) {
    printMap(out, m);
    return out;
}

std::string getLocalDateTime(const char format[]);

std::string getLocalDateTime();

inline std::size_t generateHashForString(const std::string& str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

uint64_t findClosest(const std::vector<uint64_t>& v, const uint64_t target);

void splitStr(const std::string& str, const char separator,
              std::vector<std::string>& strings);

#endif /* UTILS_HPP_ */