#include <iostream>
#include <random>

using ll = int64_t;
using ull = uint64_t;

std::mt19937 rnd(1984);

struct Date {
  ull day_ = 0, month_ = 0, year_ = 0;
  Date() {}
  Date(Date& d) {
    day_= d.day_;
    month_ = d.month_;
    year_ = d.year_;
  }
  Date (Date&& d) {
    day_ = std::exchange(d.day_, 0);
    month_ = std::exchange(d.month_, 0);
    year_ = std::exchange(d.year_, 0);
  }
  Date (ull day, ull month, ull year) : day_(day), month_(month), year_(year) {
    if (!check()) throw std::logic_error("Wrong Date");
  }
  bool check() {
    if ((bool)day_ != (bool)month_) {
      return false;
    }
    if (month_ > 12) {
      return false;
    }
    if (day_ > 31 || day_ > 30 && (month_ < 7 && month_ % 2 == 0 || month_ > 6 && month_ % 2 == 1) ||
      month_ == 2 && day_ > 28 + (year_ % 400 == 0 || year_ % 4 == 0 && year_ % 100 != 0)) {
        return false;
    }
    return true;
  }
  bool operator==(Date other) {
    return day_ == other.day_ && month_ == other.month_ && year_ == other.year_;
  }
  bool operator!=(Date other) {
    return !operator==(other);
  }
  bool operator<(Date other) {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
  }
  bool operator>(Date other) {
    return other < *this;
  }
  bool operator<=(Date other) {
    return !operator>(other);
  }
  bool operator>=(Date other) {
    return !operator<(other);
  }
  Date operator++(int) {
    Date d = *this;
    if (day_ == 30 + (month_ % 2 == 1 && month_ <= 7 || month_ % 2 == 0 && month_ > 7) ||
      month_ == 2 && day_ == 28 + (year_ % 400 == 0 || year_ % 4 == 0 && year_ % 100 != 0)) {
      if (month_ == 12) {
        ++d.year_;
        d.month_ = 1;
        d.day_ = 1;
        return d;
      } else {
        ++d.month_;
        d.day_ = 1;
      }
    } else ++d.day_;
    return d;
  }
  Date& operator++() {
    if (day_ == 30 + (month_ % 2 == 1 && month_ <= 7 || month_ % 2 == 0 && month_ > 7) ||
      month_ == 2 && day_ == 28 + (year_ % 400 == 0 || year_ % 4 == 0 && year_ % 100 != 0)) {
      if (month_ == 12) {
        ++year_;
        month_ = 1;
        day_ = 1;
        return *this;
      } else {
        ++month_;
        day_ = 1;
      }
    } else ++day_;
    return *this;
  }
  Date& operator--() {
    if (day_ == 1) {
      if (month_ == 1) {
        --year_;
        month_ = 12;
        day_ = 31;
        return *this;
      }
      --month_;
      day_ = (month_ == 2 ? 
        28 + (year_ % 400 == 0 || year_ % 4 == 0 && year_ % 100 != 0) : 
        30 + (month_ % 2 == 1 && month_ <= 7 || month_ % 2 == 0 && month_ > 7));
      return *this;
    }
    --day_;
    return *this;
  }
  Date operator--(int) {
    Date d(*this);
    if (d.day_ == 1) {
      if (d.month_ == 1) {
        --d.year_;
        d.month_ = 12;
        d.day_ = 31;
        return d;
      }
      --d.month_;
      d.day_ = (d.month_ == 2 ? 
        28 + (d.year_ % 400 == 0 || d.year_ % 4 == 0 && d.year_ % 100 != 0) : 
        30 + (d.month_ % 2 == 1 && d.month_ <= 7 || d.month_ % 2 == 0 && d.month_ > 7));
      return d;
    }
    --d.day_;
    return d;
  }
  Date operator=(Date other) {
    day_ = other.day_;
    month_ = other.month_;
    year_ = other.month_;
    return *this;
  }
};

struct String {
  char *data_ = new char[1];
  ull size_ = 0, capacity_ = 1;
  String() {}
  String(String &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other.data_[i];
  }
  String(String &&other) {
    size_ = std::move(other.size_);
    capacity_ = std::move(other.capacity_);
    if (data_) delete data_;
    data_ = std::exchange(other.data_, nullptr);
  }
  String(const char other[]) {
    size_ = sizeof other;
    capacity_ = size_ * 2;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other[i];
  }
  String(ull size, char c) {
    size_ = size;
    capacity_ = 2 * size;
    data_ = new char[capacity_];
    for (int i = 0; i < size; ++i) data_[i] = c;
  } 
  String operator=(String other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (data_) delete data_;
    data_ = new char[capacity_];
    for (int i = 0; i < size_; ++i) data_[i] = other.data_[i];
    return *this;
  }
  char& operator[](ll ind) {
    if (ind < 0 || ind >= size_) throw std::logic_error("Wrong indexation");
    return data_[ind];
  }
  String operator+=(char c) {
    push_back(c);
    return *this;
  }
  String operator+=(String str) {
    ll new_capacity = capacity_;
    while (new_capacity < size_ + str.size_) new_capacity *= 2;
    if (new_capacity != capacity_) reallocate(new_capacity);
    for (int i = 0; i < str.size_; ++i) data_[size_ + i] = str[i];
    size_ += str.size_;
    return *this;
  }
  void push_back(char c) {
    if (size_ == capacity_) {
      reallocate(capacity_ * 2);
    }
    data_[size_++] = c;
  }
  void pop_back() {
    if (!size_) throw std::logic_error("Wrong indexation");
    --size_;
  }
  void insert(ll ind, char c) {
    if (ind < 0 || ind > size_) throw std::logic_error("Wrong indexation");
    if (capacity_ == size_) reallocate(2 * capacity_);
    for (int i = size_; i > ind; --i) data_[i] = data_[i - 1];
    data_[ind] = c;
  }
  void insert(ll ind, String str) {
    ll new_capacity = capacity_;
    while (new_capacity < size_ + str.size_) new_capacity *= 2;
    if (new_capacity != capacity_) reallocate(new_capacity);
    for (int i = ind + str.size_; i < size_ + str.size_; ++i) data_[i] = data_[i - str.size_];
    for (int i = 0; i < str.size_; ++i) data_[ind + i] = str[i];
    size_ += str.size_;
  }
  void erase(ll ind) {
    if (ind < 0 || ind >= size_)  throw std::logic_error("Wring Index");
    for (int i = ind; i < size_ - 1; ++i) data_[i] = data_[i + 1];
    --size_;
  }
  void erase(ll lx, ll x) {
    if (lx < 0 || x < 0 || x + lx > size_) throw std::logic_error("Wrong indexation");
    for (int i = lx; i < size_ - x; ++i) data_[i] = data_[i + x];
    size_ -= x;
  }
  void clear() {
    size_ = 0;
  }
  void reserve(ll x) {
    if (x < size_) throw std::logic_error("Wring indexation");
    reallocate(x);
  }
  void reallocate(ll new_capacity) {
    capacity_ = new_capacity;
    char *new_data = new char[capacity_];
    for (int i = 0; i < size_; ++i) new_data[i] = data_[i];
    if (data_) delete data_;
    data_ = nullptr;
    std::swap(data_, new_data);
  }
};

struct Tree {
  struct Node {
    Date key_;
    String val_;
    Node *l_ = nullptr, *r_ = nullptr;
    Node() {}
    Node(Date key) : key_(key) {}
    ~Node() {}
  } *root = nullptr;
  Node *merge(Node *lx, Node *rx) {
    if (!lx) return rx;
    if (!rx) return lx;
    if (rnd() > rnd()) {
      lx->r_ = merge(lx->r_, rx);
      return lx;
    } else {
      rx->l_ = merge(lx, rx->l_);
      return rx;
    }
  }
  void split(Node *n, Date d, Node *&lx, Node *&rx) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    if (!n) {
      lx = rx = nullptr;
      return;
    }
    if (n->key_ < d) {
      Node *ll, *rr;
      lx = n;
      split(lx->r_, d, ll, rr);
      lx->r_ = ll;
      rx = rr;
      return;
    } else {
      Node *ll, *rr;
      rx = n;
      split(rx->l_, d, ll, rr);
      rx->l_ = rr;
      lx = ll;
      return;
    }
  }
  void erase(Date d) {
    if (!d.day_ || !d.month_) throw std::logic_error("Wring Date");
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    root = merge(less, more);
    if (eq) delete eq; 
  }
  Node* insert(Date d, String str = "") {
    if (!d.day_ || !d.month_) throw std::logic_error("Wrong Date");
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, d, less, more);
    split(root, d++, eq, more);
    if (!eq) eq = new Node(d);
    eq->val_ = str;
    root = merge(less, merge(eq, more));
    return eq; 
  }
  String& operator[](Date d) {
    Node *less = nullptr, *eq = nullptr, *more = nullptr;
    split(root, d, less, more);
    split(more, d++, eq, more);
    root = merge(less, merge(eq, more));
    return eq->val_;
  }
};