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

ll countDays(ll month, ll year) {
    if (month < 1 || month > 12) throw std::logic_error("Wrong Date");
    if (month == 2) return 28 + (year % 400 == 0 || year % 4 == 0 && year % 100 != 0);
    return 30 + (month % 2 && month < 8 || month % 2 == 0 && month > 7);
}

std::vector<std::vector<std::vector<Date>>> Year(ll year) {
    std::vector<std::vector<std::vector<Date>>> res(13);
    Date d(1, 1, year);
    ll f = ((year - 1) * 365 + (year - 1) / 400 + (year - 1) / 4 - (year - 1) / 100) % 7;
    while (f > 0) {
        --f;
        --d;
    }
    for (int i = 0; i <= 12; ++i) {
        res[i].resize(7, std::vector<Date>(6));
        for (int j = 0; j < 6; ++j) {
            for (int k = 0; k < 7; ++k) {
                res[i][k][j] = d;
                ++d;
                ++f;
            }
            f = 0;
        }
        if (i <= 11) {
            for (int j = 0; j < 7 || d > Date(1, i + 1, year) || f != 0; ++j) {
                --d;
                f = (f + 6) % 7;
            }
        }
    }
    return res;
}
