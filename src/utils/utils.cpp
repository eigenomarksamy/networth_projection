#include <ctime>
#include <iomanip>
#include <sstream>
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
    return getLocalDateTime("%d-%m-%Y %H-%M-%S");
}