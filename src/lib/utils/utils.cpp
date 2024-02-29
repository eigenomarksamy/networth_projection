#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "utils.hpp"


std::string getLocalDateTime(const char format[]) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format);
    auto str = oss.str();
    return str;
}

std::string getLocalDateTime() {
    return getLocalDateTime("%d-%m-%Y_%H-%M-%S");
}

std::string getUniqueTimeId() {
    return getLocalDateTime("%d%m%Y%H%M%S");
}

Date getDateTime() {
    auto strDateTime = getLocalDateTime();
    Date date;
    return date;
}

uint64_t findClosest(const std::vector<uint64_t>& v, const uint64_t target) {
    std::vector<uint64_t> vec = v;
    std::sort(vec.begin(), vec.end());
    size_t left = 0, right = vec.size() - 1;
    while (left < right) {
        if (fabs(vec[left] - target) < fabs(vec[right] - target)) {
            right--;
        }
        else {
            left++;
        }
    }
    return vec[left];
}

void splitStr(const std::string& str, const char separator,
              std::vector<std::string>& strings) {
    size_t startIdx = 0, endIdx = 0;
    for (size_t i = 0; i <= str.size(); ++i) {
        if (str[i] == separator || i == str.size()) {
            endIdx = i;
            std::string tmp;
            tmp.append(str, startIdx, endIdx - startIdx);
            strings.push_back(tmp);
            startIdx = endIdx + 1;
        }
    }
}

std::string convertVectorToString(const std::vector<std::string>& vec,
                                  const std::string& separator) {
    std::string str = "";
    for (uint16_t v = 0; v < vec.size() - 1; ++v) {
        str += vec[v] + separator;
    }
    str += vec[vec.size() - 1];
    return str;
}

bool convertStrToBool(const std::string& str) {
    return convertStrToBool(str, false);
}

bool convertStrToBool(const std::string& str, const bool defaultValue) {
    std::string s = str;
    for (auto& c : s) {
        c = tolower(c);
    }
    if (s == "false") {
        return false;
    }
    else if (s == "true") {
        return true;
    }
    return defaultValue;
}