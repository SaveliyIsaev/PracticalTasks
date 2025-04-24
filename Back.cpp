#include "Back.hpp"


//Vector input/output operators
template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& a) {
    for (T& x : a) in >> x;
    return in;
}
template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> a) {
    for (T& x : a) out << x << ' ';
    return out;
}
template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<std::vector<T>> a) {
    for (std::vector<T>& x : a) out << x << '\n';
    return out;
}

//Date
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
    if (day_ < 1 || day_ > countDays(month_, year_));
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
/*
//String
String::String() {}
String::String(String& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other.data_[i];
}
String::String(String&& other) {
    size_ = std::move(other.size_);
    capacity_ = std::move(other.capacity_);
    if (data_) delete data_;
    data_ = std::exchange(other.data_, nullptr);
}
String::String(const char other[]) {
    size_ = sizeof other;
    capacity_ = size_ * 2;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other[i];
}
String::String(ull size, char c) {
    size_ = size;
    capacity_ = 2 * size;
    data_ = new char[capacity_];
    for (int i = 0; i < size; ++i) data_[i] = c;
}
String String::operator=(String other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other.data_[i];
    return *this;
}
char& String::operator[](ll ind) {
    if (ind < 0 || ind >= size_) throw std::logic_error("Wrong indexation");
    return data_[ind];
}
std::istream& operator>>(std::istream& in, String& s) {
    char c;
    while (in.get(c)) {
        if (c > 32 && c < 128) s += c;
        else break;
    }
    return in;
}
std::ostream& operator<<(std::ostream& out, String s) {
    for (int i = 0; i < s.size_; ++i) out << s[i];
    return out;
}
String String::operator+=(char c) {
    push_back(c);
    return *this;
}
String String::operator+=(String str) {
    ll new_capacity = capacity_;
    while (new_capacity < size_ + str.size_) new_capacity *= 2;
    if (new_capacity != capacity_) reallocate(new_capacity);
    for (int i = 0; i < str.size_; ++i) data_[size_ + i] = str[i];
    size_ += str.size_;
    return *this;
}
String::operator char* () const {
    char* c = new char[size_ - 1];
    for (int i = 0; i < size_; ++i) c[i] = data_[i];
    return c;
}
void String::push_back(char c) {
    if (size_ == capacity_) {
        reallocate(capacity_ * 2);
    }
    data_[size_++] = c;
}
void String::pop_back() {
    if (!size_) throw std::logic_error("Wrong indexation");
    --size_;
}
void String::insert(ll ind, char c) {
    if (ind < 0 || ind > size_) throw std::logic_error("Wrong indexation");
    if (capacity_ == size_) reallocate(2 * capacity_);
    for (int i = size_; i > ind; --i) data_[i] = data_[i - 1];
    data_[ind] = c;
}
void String::insert(ll ind, String str) {
    ll new_capacity = capacity_;
    while (new_capacity < size_ + str.size_) new_capacity *= 2;
    if (new_capacity != capacity_) reallocate(new_capacity);
    for (int i = ind + str.size_; i < size_ + str.size_; ++i) data_[i] = data_[i - str.size_];
    for (int i = 0; i < str.size_; ++i) data_[ind + i] = str[i];
    size_ += str.size_;
}
void String::erase(ll ind) {
    if (ind < 0 || ind >= size_)  throw std::logic_error("Wring Index");
    for (int i = ind; i < size_ - 1; ++i) data_[i] = data_[i + 1];
    --size_;
}
void String::erase(ll lx, ll x) {
    if (lx < 0 || x < 0 || x + lx > size_) throw std::logic_error("Wrong indexation");
    for (int i = lx; i < size_ - x; ++i) data_[i] = data_[i + x];
    size_ -= x;
}
void String::clear() {
    size_ = 0;
}
void String::reserve(ll x) {
    if (x < size_) throw std::logic_error("Wring indexation");
    reallocate(x);
}
void String::reallocate(ll new_capacity) {
    capacity_ = new_capacity;
    char* new_data = new char[capacity_];
    for (int i = 0; i < size_; ++i) new_data[i] = data_[i];
    if (data_) delete data_;
    data_ = nullptr;
    std::swap(data_, new_data);
}
*/
//Decart Tree
Tree::Node::Node() {}
Tree::Node::Node(Date key) : key_(key) {}
Tree::Node::~Node() {}
Tree::Node* Tree::merge(Node* lx, Node* rx) {
    if (!lx) return rx;
    if (!rx) return lx;
    if (rnd() > rnd()) {
        lx->r_ = merge(lx->r_, rx);
        return lx;
    }
    else {
        rx->l_ = merge(lx, rx->l_);
        return rx;
    }
}
void Tree::split(Node* n, Date d, Node*& lx, Node*& rx, ll x = 0) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    if (!n) {
        lx = rx = nullptr;
        return;
    }
    std::cout << x << std::endl;
    if (n->key_ < d) {
        Node* ll = nullptr, * rr = nullptr;
        lx = n;
        split(lx->r_, d, ll, rr, x + 1);
        lx->r_ = ll;
        rx = rr;
        return;
    }
    else {
        Node *ll = nullptr, *rr = nullptr;
        rx = n;
        split(rx->l_, d, ll, rr, x + 1);
        rx->l_ = rr;
        lx = ll;
        return;
    }
}
void Tree::erase(Date d) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wring Date");
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    root = merge(less, more);
    if (eq) delete eq;
}
Tree::Node* Tree::update(Date d, std::wstring str) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(root, d++, eq, more);
    if (!eq) eq = new Node(d);
    eq->val_ = str;
    root = merge(less, merge(eq, more));
    return eq;
}
std::wstring Tree::value(Date d) {
    Node* less = nullptr, * eq = nullptr, * more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    std::wstring res;
    if (eq) res = eq->val_;
    root = merge(less, merge(eq, more));
    return res;
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
            for (int j = 0;  j < 7 || d > Date(1, i + 1, year) || f != 0; ++j) {
                --d;
                f = (f + 6) % 7;
            }
        }
    }
    return res;
}

ll countDays(ll month, ll year) {
    if (month < 1 || month > 12) throw std::logic_error("Wrong Date");
    if (month == 2) return 28 + (year % 400 == 0 || year % 4 == 0 && year % 100 != 0);
    return 30 + (month % 2 && month < 8 || month % 2 == 0 && month > 7);
}