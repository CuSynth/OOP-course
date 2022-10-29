#pragma once
#include <sstream>
#include <locale>
#include <iomanip>
#include <chrono>

class TimeIO {
public:
    // Let's consider we are getting all the input in
    // this format: '2014-07-25T20:17:22Z' (T denotes
    // start of Time part, Z denotes UTC zone).
    // A better approach would be to pass in the format as well.
    //#define dateTimeFormat "%Y-%m-%dT%H:%M:%SZ"

#define dateTimeFormat "%Y-%m-%dT%H:%M:%S"

    static std::string time_str(time_t tm) {
        std::ostringstream ss{};
        std::tm dt; localtime_s(&dt, &tm);
        ss << std::put_time(&dt, dateTimeFormat);
        return ss.str();
    }
    static time_t parse_time(const char* time) {
        std::istringstream ss{ time };
        std::tm dt{};

        if (!(ss >> std::get_time(&dt, dateTimeFormat)) || ss.fail())
            throw std::invalid_argument("wrong time format");

        return std::mktime(&dt);
    }

    static std::string time_str(const std::chrono::system_clock::time_point& tp) {
        return time_str(std::chrono::system_clock::to_time_t(tp));
    }
    static std::chrono::system_clock::time_point parse_system_clock(const char* time) {
        return std::chrono::system_clock::from_time_t(parse_time(time));
    }
};

namespace std {
    inline std::istream& operator>>(std::istream & is, std::chrono::system_clock::time_point & tp) {
        std::string strt;
        try {
            if (is >> strt && !strt.empty()) {
                tp = TimeIO::parse_system_clock(strt.c_str());
            }
            else {
                throw std::invalid_argument("wrong time format");
            }
        }
        catch (const std::invalid_argument&) {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    inline std::ostream& operator<<(std::ostream & os, const std::chrono::system_clock::time_point & tp)
    {
        os << TimeIO::time_str(tp);
        return os;
    }
}
