#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "datetime.hpp"

Date::Date(uint16_t year) {
    m_year = year;
    m_mode = DatePrecision::Year;
}

Date::Date(uint16_t month, uint16_t year) {
    m_month = month;
    m_year = year;
    m_mode = DatePrecision::Month;
}

Date::Date(uint16_t day, uint16_t month, uint16_t year) {
    m_day = day;
    m_month = month;
    m_year = year;
    m_mode = DatePrecision::Day;
}

bool Date::operator==(const Date& other) const {
    return ((m_year == other.m_year)
            && (m_mode == other.m_mode)
            && ((m_mode == DatePrecision::Year)
                || ((m_mode == DatePrecision::Month)
                    && (m_month == other.m_month))
                || ((m_mode == DatePrecision::Day)
                    && (m_month == other.m_month)
                    && (m_day == other.m_day))));
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator>(const Date& other) const {
    if (m_year < other.m_year) return false;
    if (DatePrecision::Month == m_mode) {
        if (m_month < other.m_month) return false;
    }
    if (DatePrecision::Day == m_mode) {
        if (m_month < other.m_month) return false;
        if (m_day < other.m_day) return false;
    }
    return true;
}

bool Date::operator<(const Date& other) const {
    return !(*this > other || *this == other);
}

bool Date::operator>=(const Date& other) const {
    return (*this == other || *this > other);
}

bool Date::operator<=(const Date& other) const {
    return (*this == other || *this < other);
}

Time::Time(uint16_t hour) {
    m_hour = hour;
    m_mode = TimePrecision::Hour;
}

Time::Time(uint16_t minute, uint16_t hour) {
    m_minute = minute;
    m_hour = hour;
    m_mode = TimePrecision::Minute;
}

Time::Time(uint16_t second, uint16_t minute, uint16_t hour) {
    m_second = second;
    m_minute = minute;
    m_hour = hour;
    m_mode = TimePrecision::Second;
}

bool Time::operator==(const Time& other) const{
    return ((m_hour == other.m_hour)
            && (m_mode == other.m_mode)
            && ((m_mode == TimePrecision::Hour)
                || ((m_mode == TimePrecision::Minute)
                    && (m_minute == other.m_minute))
                || ((m_mode == TimePrecision::Second)
                    && (m_minute == other.m_minute)
                    && (m_second == other.m_second))));
}

bool Time::operator!=(const Time& other) const{
    return !(*this == other);
}

bool Time::operator>(const Time& other) const{
    if (m_hour < other.m_hour) return false;
    if (TimePrecision::Minute == m_mode) {
        if (m_minute < other.m_minute) return false;
    }
    if (TimePrecision::Second == m_mode) {
        if (m_minute < other.m_minute) return false;
        if (m_second < other.m_second) return false;
    }
    return true;
}

bool Time::operator<(const Time& other) const{
    return !(*this > other || *this == other);
}

bool Time::operator>=(const Time& other) const {
    return (*this == other || *this > other);
}

bool Time::operator<=(const Time& other) const {
    return (*this == other || *this < other);
}

DateTime::DateTime(uint16_t year) {
    m_date = Date(year);
    m_mode = DateTimePrecision::Year;
}

DateTime::DateTime(uint16_t month, uint16_t year) {
    m_date = Date(month, year);
    m_mode = DateTimePrecision::Month;
}

DateTime::DateTime(uint16_t day, uint16_t month, uint16_t year) {
    m_date = Date(day, month, year);
    m_mode = DateTimePrecision::Day;
}

DateTime::DateTime(uint16_t hour, uint16_t day, uint16_t month, uint16_t year) {
    m_date = Date(day, month, year);
    m_time = Time(hour);
    m_mode = DateTimePrecision::Hour;
}

DateTime::DateTime(uint16_t minute, uint16_t hour, uint16_t day, uint16_t month, uint16_t year) {
    m_date = Date(day, month, year);
    m_time = Time(minute, hour);
    m_mode = DateTimePrecision::Minute;
}

DateTime::DateTime(uint16_t second, uint16_t minute, uint16_t hour, uint16_t day, uint16_t month, uint16_t year) {
    m_date = Date(day, month, year);
    m_time = Time(second, minute, hour);
    m_mode = DateTimePrecision::Second;
}

bool DateTime::operator==(const DateTime& other) const {
    return (m_mode == other.m_mode)
            && (m_date == other.m_date)
            && ((m_mode == DateTimePrecision::Day
                 || m_mode == DateTimePrecision::Month
                 || m_mode == DateTimePrecision::Year)
                || ((m_mode == DateTimePrecision::Second)
                    || (m_mode == DateTimePrecision::Minute)
                    || (m_mode == DateTimePrecision::Day)
                    && (m_time == other.m_time)));
}

bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
}

bool DateTime::operator>(const DateTime& other) const {
    if (m_date < other.m_date) return false;
    if (DateTimePrecision::Second == m_mode
        || DateTimePrecision::Minute == m_mode
        || DateTimePrecision::Hour == m_mode) {
        if (m_time < other.m_time) return false;
    }
    return true;
}

bool DateTime::operator<(const DateTime& other) const {
    return !(*this > other || *this == other);
}

bool DateTime::operator>=(const DateTime& other) const {
    return (*this == other || *this > other);
}

bool DateTime::operator<=(const DateTime& other) const {
    return (*this == other || *this < other);
}

std::string DateTime::getLocalDateTime(const char format[]) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, format);
    auto str = oss.str();
    return str;
}

std::string DateTime::getLocalDateTime() {
    return DateTime::getLocalDateTime("%d-%m-%Y_%H-%M-%S");
}

std::string DateTime::getUniqueTimeId() {
    return DateTime::getLocalDateTime("%d%m%Y%H%M%S");
}

std::string DateTime::convertDateToString(const Date& date) {
    std::string date_str;
    date_str = std::to_string(date.m_year);
    if (date.m_mode == Date::DatePrecision::Month) {
        if (date.m_month < 10) {
            date_str = "0" + std::to_string(date.m_month) + date_str;
        }
        else {
            date_str = std::to_string(date.m_month) + date_str;
        }
    }
    else if (date.m_mode == Date::DatePrecision::Day) {
        if (date.m_month < 10) {
            date_str = "0" + std::to_string(date.m_month) + date_str;
        }
        else {
            date_str = std::to_string(date.m_month) + date_str;
        }
        if (date.m_day < 10) {
            date_str = "0" + std::to_string(date.m_day) + date_str;
        }
        else {
            date_str = std::to_string(date.m_day) + date_str;
        }
    }
    return date_str;
}

std::string DateTime::convertTimeToString(const Time& time) {
    std::string time_str;
    time_str = std::to_string(time.m_hour);
    if (time.m_mode == Time::TimePrecision::Minute) {
        if (time.m_minute < 10) {
            time_str = "0" + std::to_string(time.m_minute) + time_str;
        }
        else {
            time_str = std::to_string(time.m_minute) + time_str;
        }
    }
    else if (time.m_mode == Time::TimePrecision::Second) {
        if (time.m_minute < 10) {
            time_str = "0" + std::to_string(time.m_minute) + time_str;
        }
        else {
            time_str = std::to_string(time.m_minute) + time_str;
        }
        if (time.m_second < 10) {
            time_str = "0" + std::to_string(time.m_second) + time_str;
        }
        else {
            time_str = std::to_string(time.m_second) + time_str;
        }
    }
    return time_str;
}

std::string DateTime::convertDateTimeToString(const DateTime& datetime) {
    std::string datetime_str = "";
    if (datetime.m_mode == DateTimePrecision::Hour
        || datetime.m_mode == DateTimePrecision::Minute
        || datetime.m_mode == DateTimePrecision::Second) {
        datetime_str += convertTimeToString(datetime.m_time);
    }
    datetime_str += convertDateToString(datetime.m_date);
    return datetime_str;
}

DateTime DateTime::getDateTimeNow() {
    DateTime datetime;
    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    struct std::tm *timeInfo = std::localtime(&timePoint);
    datetime.setTime(Time(static_cast<uint16_t>(timeInfo->tm_sec),
                          static_cast<uint16_t>(timeInfo->tm_min),
                          static_cast<uint16_t>(timeInfo->tm_hour)));
    datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                          static_cast<uint16_t>(timeInfo->tm_mon + 1),
                          static_cast<uint16_t>(timeInfo->tm_year + 1900)));
    datetime.setMode(DateTimePrecision::Second);
    return datetime;
}

DateTime DateTime::getDateTimeNow(const DateTimePrecision mode) {
    DateTime datetime;
    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    struct std::tm *timeInfo = std::localtime(&timePoint);
    if (mode == DateTimePrecision::Second) {
        datetime = getDateTimeNow();
    }
    else if (mode == DateTimePrecision::Minute) {
        datetime.setTime(Time(static_cast<uint16_t>(timeInfo->tm_min),
                              static_cast<uint16_t>(timeInfo->tm_hour)));
        datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                              static_cast<uint16_t>(timeInfo->tm_mon + 1),
                              static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        datetime.setMode(mode);
    }
    else if (mode == DateTimePrecision::Hour) {
        datetime.setTime(Time(static_cast<uint16_t>(timeInfo->tm_hour)));
        datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                              static_cast<uint16_t>(timeInfo->tm_mon + 1),
                              static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        datetime.setMode(mode);
    }
    else if (mode == DateTimePrecision::Day) {
        datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                              static_cast<uint16_t>(timeInfo->tm_mon + 1),
                              static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        datetime.setMode(mode);
    }
    else if (mode == DateTimePrecision::Month) {
        datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_mon + 1),
                              static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        datetime.setMode(mode);
    }
    else {
        datetime.setDate(Date(static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        datetime.setMode(mode);
    }
    return datetime;
}

std::string DateTime::getDateTimeString() const {
    return convertDateTimeToString(*this);
}

void DateTime::setToNow() {
    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    struct std::tm *timeInfo = std::localtime(&timePoint);
    setTime(Time(static_cast<uint16_t>(timeInfo->tm_sec),
                 static_cast<uint16_t>(timeInfo->tm_min),
                 static_cast<uint16_t>(timeInfo->tm_hour)));
    setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                 static_cast<uint16_t>(timeInfo->tm_mon + 1),
                 static_cast<uint16_t>(timeInfo->tm_year + 1900)));
    setMode(DateTimePrecision::Second);
}

void DateTime::setToNow(const DateTimePrecision mode) {
    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    struct std::tm *timeInfo = std::localtime(&timePoint);
    if (mode == DateTimePrecision::Second) {
        setTime(Time(static_cast<uint16_t>(timeInfo->tm_sec),
                     static_cast<uint16_t>(timeInfo->tm_min),
                     static_cast<uint16_t>(timeInfo->tm_hour)));
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                     static_cast<uint16_t>(timeInfo->tm_mon + 1),
                     static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
    else if (mode == DateTimePrecision::Minute) {
        setTime(Time(static_cast<uint16_t>(timeInfo->tm_min),
                     static_cast<uint16_t>(timeInfo->tm_hour)));
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                     static_cast<uint16_t>(timeInfo->tm_mon + 1),
                     static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
    else if (mode == DateTimePrecision::Hour) {
        setTime(Time(static_cast<uint16_t>(timeInfo->tm_hour)));
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                     static_cast<uint16_t>(timeInfo->tm_mon + 1),
                     static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
    else if (mode == DateTimePrecision::Day) {
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_mday),
                     static_cast<uint16_t>(timeInfo->tm_mon + 1),
                     static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
    else if (mode == DateTimePrecision::Month) {
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_mon + 1),
                     static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
    else {
        setDate(Date(static_cast<uint16_t>(timeInfo->tm_year + 1900)));
        setMode(mode);
    }
}

bool DateTime::validate() const {
    return (*this <= getDateTimeNow(m_mode));
}
