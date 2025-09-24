#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#pragma once

#include "Monomial.hpp"
#include "set.hpp"
#include "List.hpp"
#include <vector>
#include <iostream>
#include <bitset>
//#include <QPair>
class Polynomial {
public:
    Polynomial();
    Polynomial(Polynomial&);
    Polynomial(Polynomial&&);
    Polynomial(set<Monomial>&);
    Polynomial(set<Monomial>&&);
    ~Polynomial();
    Polynomial& operator=(Polynomial& m);
    Polynomial& operator=(Polynomial&& m);
    Polynomial operator+();
    Polynomial operator-();
    Polynomial operator+(Polynomial);
    Polynomial operator-(Polynomial);
    Polynomial operator*(Polynomial);
    std::pair<Polynomial, Polynomial> operator/(Polynomial);
    Polynomial& operator+=(Polynomial);
    Polynomial& operator-=(Polynomial);
    Polynomial& operator*=(Polynomial);
    Polynomial& operator/=(Polynomial);
    Polynomial operator+(Monomial);
    Polynomial operator-(Monomial);
    Polynomial operator*(Monomial);
    Polynomial operator/(Monomial);
    Polynomial& operator+=(Monomial);
    Polynomial& operator-=(Monomial);
    Polynomial& operator*=(Monomial);
    Polynomial& operator/=(Monomial);
    ld operator[](std::vector<ld>);
    bool operator==(Polynomial);
    bool operator!=(Polynomial);
    bool operator<(Polynomial);
    bool operator>(Polynomial);
    bool operator<=(Polynomial);
    bool operator>=(Polynomial);
    Polynomial derivative(ll, ll);
    operator std::string();
    operator std::wstring();
    friend std::istream& operator>>(std::istream&, Polynomial&);
    friend std::ostream& operator<<(std::ostream& s, Polynomial p) {
        return s << (std::string)p;
    }
    std::bitset<26> used();
    ll size();
private:
    set<Monomial> mons;
};

#endif // POLYNOMIAL_HPP
