#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <math.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cstdint>
#include <ctime>
#include <chrono>

inline void convertStrToLowerCase(std::string& str) {
    for (auto& c : str) {
        c = tolower(c);
    }
}

struct Date {
    uint16_t m_second;
    uint16_t m_minute;
    uint16_t m_hour;
    uint16_t m_day;
    uint16_t m_month;
    uint16_t m_year;
    Date() : m_second(1), m_minute(1), m_hour(0), m_day(1), m_month(1), m_year(1970) {}
    Date(const uint16_t second, const uint16_t minute, const uint16_t hour,
         const uint16_t day, const uint16_t month, const uint16_t year)
      : m_second(second), m_minute(minute), m_hour(hour),
        m_day(day), m_month(month), m_year(year) {}
    Date(const uint16_t second, const uint16_t minute, const uint16_t hour,
         uint16_t day, const std::string& month, const uint16_t year)
      : m_second(second), m_minute(minute), m_hour(hour),
        m_day(day), m_year(year) {
        std::string month_in = month;
        convertStrToLowerCase(month_in);
        if (month_in == "january") m_month = 1;
        else if (month_in == "february") m_month = 2;
        else if (month_in == "march") m_month = 3;
        else if (month_in == "april") m_month = 4;
        else if (month_in == "may") m_month = 5;
        else if (month_in == "june") m_month = 6;
        else if (month_in == "july") m_month = 7;
        else if (month_in == "august") m_month = 8;
        else if (month_in == "september") m_month = 9;
        else if (month_in == "october") m_month = 10;
        else if (month_in == "november") m_month = 11;
        else m_month = 12;
    }
    Date(const std::string& date, const std::string& format) {

    }
    bool operator==(const Date& other) const {
        return ((m_second == other.m_second) &&
                (m_minute == other.m_minute) &&
                (m_hour == other.m_hour) &&
                (m_day == other.m_day) &&
                (m_month == other.m_month) &&
                (m_year == other.m_year));
    }
    bool operator!=(const Date& other) const {
        return !(*this == other);
    }
    void now() {
        auto now = std::chrono::system_clock::now();
        auto timePoint = std::chrono::system_clock::to_time_t(now);
        struct std::tm *timeInfo = std::localtime(&timePoint);
        m_second = static_cast<uint16_t>(timeInfo->tm_sec);
        m_minute = static_cast<uint16_t>(timeInfo->tm_min);
        m_hour = static_cast<uint16_t>(timeInfo->tm_hour);
        m_day = static_cast<uint16_t>(timeInfo->tm_mday);
        m_month = static_cast<uint16_t>(timeInfo->tm_mon + 1);
        m_year = static_cast<uint16_t>(timeInfo->tm_year + 1900);
    }
};

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
void printMap(std::ostream& out, std::map<K, V> const &m) {
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K, V>& m) {
    printMap(out, m);
    return out;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& m) {
    printMap(out, m);
    return out;
}

template <typename T>
T convertToNumeric(const std::string& str) {
    if (str.empty()) {
        return 0;
    }
    T result;
    std::stringstream ss(str);
    ss >> result;
    return result;
}

using NumericValue = std::variant<int32_t, double_t>;

template <typename T>
T convertToNumeric(const std::string& str, const T& defaultValue) {
    if (str.empty()) {
        return defaultValue;
    }
    T result;
    try {
        size_t pos;
        if (str.find('.') != std::string::npos) {
            double_t doubleValue = std::stod(str, &pos);
            if (pos == str.size())
                result = doubleValue;
        } else {
            int32_t intValue = std::stoi(str, &pos);
            if (pos == str.size())
                result = intValue;
        }
    } catch (const std::exception&) {
        return defaultValue;
    }
    return result;
}

bool convertStrToBool(const std::string& str);

bool convertStrToBool(const std::string& str, const bool defaultValue);

std::string getLocalDateTime(const char format[]);

std::string getLocalDateTime();

std::string getUniqueTimeId();

Date getDateTime();

inline std::size_t generateHashForString(const std::string& str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

uint64_t findClosest(const std::vector<uint64_t>& v, const uint64_t target);

void splitStr(const std::string& str, const char separator,
              std::vector<std::string>& strings);

std::string convertVectorToString(const std::vector<std::string>& vec,
                                  const std::string& separator=", ");

#endif /* UTILS_HPP_ */