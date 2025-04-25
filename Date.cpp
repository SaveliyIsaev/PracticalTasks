#include "Date.hpp"

Date::Date() {}
Date::Date(Date& d) {
    day_ = d.day_;
    month_ = d.month_;
    year_ = d.year_;
}
Date::Date(Date&& d) {
    day_ = std::exchange(d.day_, 0);
    month_ = std::exchange(d.month_, 0);
    year_ = std::exchange(d.year_, 0);
}
Date::Date(ull day, ull month, ull year) : day_(day), month_(month), year_(year) {
    if (!check()) {
        std::cout << day << ' ' << month << ' ' << year;
        throw std::logic_error("Wrong Date");
    }
}
bool Date::check() {
    if ((bool)day_ != (bool)month_) {
        return false;
    }
    if (month_ > 12) {
        return false;
    }
    if (day_ < 1ll || day_ > countDays(month_, year_)) return false;
    return true;
}
bool Date::operator==(Date other) {
    return day_ == other.day_ && month_ == other.month_ && year_ == other.year_;
}
bool Date::operator!=(Date other) {
    return !operator==(other);
}
bool Date::operator<(Date other) {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}
bool Date::operator>(Date other) {
    return other < *this;
}
bool Date::operator<=(Date other) {
    return !operator>(other);
}
bool Date::operator>=(Date other) {
    return !operator<(other);
}
Date Date::operator=(Date other) {
    day_ = other.day_;
    month_ = other.month_;
    year_ = other.year_;
    return *this;
}
Date::operator std::string() const {
    return std::to_string(day_) + "." + std::to_string(month_) + "." + std::to_string(year_);
}
