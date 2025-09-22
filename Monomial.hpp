#pragma once

#include <vector>
#include <string>
#include <bitset>
#include "math.hpp"

using ll = long long;
using ld = long double;
ld fastpow(ld x, ll m) {
 ld ans = 1;
 for (ll i = 1; i <= m; ++i) {
  if (i & m) ans *= x;
  x *= x;
 }
 return ans;
}

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
 private:
  std::vector<ll> pows;
  ld coef;
};

