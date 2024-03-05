#ifndef DATETIME_HPP_
#define DATETIME_HPP_

#include <string>

struct Date {
    enum class DatePrecision {Day, Month, Year};
    uint16_t m_day;
    uint16_t m_month;
    uint16_t m_year;
    DatePrecision m_mode;
    Date() = default;
    Date(uint16_t year);
    Date(uint16_t month, uint16_t year);
    Date(uint16_t day, uint16_t month, uint16_t year);
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator<=(const Date& other) const;
};

struct Time {
    enum class TimePrecision {Second, Minute, Hour};
    uint16_t m_hour;
    uint16_t m_minute;
    uint16_t m_second;
    TimePrecision m_mode;
    Time() = default;
    Time(uint16_t hour);
    Time(uint16_t minute, uint16_t hour);
    Time(uint16_t second, uint16_t minute, uint16_t hour);
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator<=(const Time& other) const;
};

class DateTime {
public:
    enum class DateTimePrecision {Second, Minute, Hour, Day, Month, Year};
    DateTime () = default;
    DateTime(uint16_t year);
    DateTime(uint16_t month, uint16_t year);
    DateTime(uint16_t day, uint16_t month, uint16_t year);
    DateTime(uint16_t hour, uint16_t day, uint16_t month, uint16_t year);
    DateTime(uint16_t minute, uint16_t hour, uint16_t day, uint16_t month, uint16_t year);
    DateTime(uint16_t second, uint16_t minute, uint16_t hour, uint16_t day, uint16_t month, uint16_t year);
    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    static std::string getLocalDateTime(const char format[]);
    static std::string getLocalDateTime();
    static std::string getUniqueTimeId();
    static std::string convertDateToString(const Date& date);
    static std::string convertTimeToString(const Time& time);
    static std::string convertDateTimeToString(const DateTime& datetime);
    static DateTime getDateTimeNow();
    static DateTime getDateTimeNow(const DateTimePrecision mode);
    void setDate(const Date& date) { m_date = date; }
    void setTime(const Time& time) { m_time = time; }
    Date getDate() const { return m_date; }
    Time getTime() const { return m_time; }
    std::string getDateTimeString() const;
    void setMode(const DateTimePrecision mode) { m_mode = mode; }
    DateTimePrecision getMode() const { return m_mode; }
    void setToNow();
    void setToNow(const DateTimePrecision mode);
    bool validate() const;
private:
    DateTimePrecision m_mode;
    Time m_time;
    Date m_date;
};

#endif /* DATETIME_HPP_ */