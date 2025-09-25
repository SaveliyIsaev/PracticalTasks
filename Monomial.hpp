#pragma once

#ifndef MONOMIAL_HPP
#define MONOMIAL_HPP

#include <vector>
#include <string>
#include <bitset>

using ll = long long;
using ld = long double;

class Monomial {
public:
    Monomial();
    Monomial(Monomial&);
    Monomial(Monomial&&);
    Monomial(ld, std::vector<ll>);
    Monomial(ld);
    ~Monomial() {}
    Monomial& operator=(Monomial& m);
    Monomial& operator=(Monomial&& m);
    Monomial operator+();
    Monomial operator-();
    Monomial operator+(Monomial);
    Monomial operator-(Monomial);
    Monomial operator*(Monomial);
    Monomial operator/(Monomial);
    Monomial& operator+=(Monomial);
    Monomial& operator-=(Monomial);
    Monomial& operator*=(Monomial);
    Monomial& operator/=(Monomial);
    ld operator[](std::vector<ld>);
    bool operator==(Monomial);
    bool operator!=(Monomial);
    bool operator<(Monomial);
    bool operator>(Monomial);
    bool operator<=(Monomial);
    bool operator>=(Monomial);
    Monomial derivative(ll, ll);
    operator std::string();
    operator std::wstring();
    friend std::stringstream& operator>>(std::stringstream&, Monomial&);
    friend std::ostream& operator<<(std::ostream&, Monomial);
    std::bitset<26> used();
    ld getCoef() {
        return coef;
    }
    std::vector<ll> getPows() {
        return pows;
    }
private:
    std::vector<ll> pows;
    ld coef;
};

#endif // MONOMIAL_HPP
