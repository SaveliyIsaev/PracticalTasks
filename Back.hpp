#pragma once

#include <iostream>
#include <random>
#include <vector>
#include <string>

using ll = int64_t;
using ull = uint64_t;
template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T>& a);
template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> a);
template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<std::vector<T>> a);

ll countDays(ll month, ll year);

struct Date {
    ull day_ = 0, month_ = 0, year_ = 0;
    Date();
    Date(Date& d);
    Date(Date&& d);
    Date(ull day, ull month, ull year);
    bool check();
    bool operator==(Date other);
    bool operator!=(Date other);
    bool operator<(Date other);
    bool operator>(Date other);
    bool operator<=(Date other);
    bool operator>=(Date other);
    Date operator++(int);
    Date& operator++();
    Date& operator--();
    Date operator--(int);
    Date operator=(Date other);
    operator std::string() const;
};
/*
struct String {
    char* data_ = new char[1];
    ull size_ = 0, capacity_ = 1;
    String();
    String(String& other);
    String(String&& other);
    String(const char other[]);
    String(ull size, char c);
    String operator=(String other);
    char& operator[](ll ind);
    String operator+=(char c);
    String operator+=(String str);
    operator char* () const;
    void push_back(char c);
    void pop_back();
    void insert(ll ind, char c);
    void insert(ll ind, String str);
    void erase(ll ind);
    void erase(ll lx, ll x);
    void clear();
    void reserve(ll x);
    void reallocate(ll new_capacity);
};

std::istream& operator>>(std::istream& in, String& s);
std::ostream& operator<<(std::ostream& out, String s);
*/
struct Tree {
    std::mt19937 rnd;
    struct Node {
        Date key_;
        std::wstring val_;
        Node* l_ = nullptr, * r_ = nullptr;
        Node();
        Node(Date key);
        ~Node();
    } *root = nullptr;
    Node* merge(Node* lx, Node* rx);
    void split(Node* n, Date d, Node*& lx, Node*& rx, ll);
    void erase(Date d);
    Node* update(Date d, std::wstring str);
    std::wstring value(Date d);
    Tree() {}
};

std::vector<std::vector<std::vector<Date>>> Year(ll year);
