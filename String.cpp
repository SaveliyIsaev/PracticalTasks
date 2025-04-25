#include "String.hpp"

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
