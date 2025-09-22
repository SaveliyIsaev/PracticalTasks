#include <exception>
#include <sstream>
#include <iostream>
#include "Monomial.hpp"

using std::endl;

Monomial::Monomial() : pows(26, 0), coef(0) {}

Monomial::Monomial(Monomial &m) :pows(m.pows), coef(m.coef) {}

Monomial::Monomial(Monomial &&m) : pows(std::move(m.pows)), coef(std::move(m.coef)) {}

Monomial::Monomial(ld cf, std::vector<ll> pws) : pows(pws), coef(cf) {}

Monomial::Monomial(ld cf) : coef(cf), pows(26, 0) {}


Monomial Monomial::operator+() {
  return *this;
}

Monomial Monomial::operator-() {
  return Monomial(-coef, pows);
}

Monomial Monomial::operator+(Monomial m) {
  if (coef == 0) return m;
  if (m.coef == 0) return *this;
  for (int i = 0; i < 26; ++i) {
    if (m.pows[i] != pows[i]) throw "NOT FUCKING SIMILAR U STUPID PIECE O' SHIT";
  }
  return Monomial(coef + m.coef, pows);
}

Monomial Monomial::operator-(Monomial m) {
  return operator+(-m);
}

Monomial Monomial::operator*(Monomial m) {
  std::vector<ll> pws(26);
  for (int i = 0; i < 26; ++i) pws[i] = pows[i] + m.pows[i];
  return Monomial(coef * m.coef, pws);
}

Monomial Monomial::operator/(Monomial m) {
  if (m.coef == 0) throw "GET THE FUCK OUT OF HERE U SHIT, I WILL NOT DIVIDE THIS BY ZERO, BUT I WILL DIVIDE MY WRATH IF U CONTINUE DOING THIS";
  std::vector<ll> pws(26);
  for (int i = 0; i < 26; ++i) {
    if (pows[i] < m.pows[i]) throw "DIE STUPID BASTARD, POWERS CAN BE ONLY NATURAL OR ZERO AND NOT FUCKING NEGATIVE";
    pws[i] = pows[i] - m.pows[i];
  }
  return Monomial(coef / m.coef, pws);
}

Monomial& Monomial::operator=(Monomial& m) {
  pows = m.pows;
  coef = m.coef;
  return *this;
}

Monomial& Monomial::operator=(Monomial&& m) {
  std::swap(m.coef, coef);
  std::swap(pows, m.pows);
  return *this;
}

Monomial& Monomial::operator+=(Monomial m) {
  return *this = *this + m;
}

Monomial& Monomial::operator-=(Monomial m) {
  return *this = *this - m;
}

Monomial& Monomial::operator*=(Monomial m) {
  return *this = *this * m;
}

Monomial& Monomial::operator/=(Monomial m) {
  return *this = *this / m;
}

ld Monomial::operator[](std::vector<ld> p) {
  ld ans = coef;
  for (int i = 0; i < 26; ++i) ans *= fastpow(p[i], pows[i]);
  return ans;
}

bool Monomial::operator==(Monomial m) {
  if (coef != m.coef) return false;
  if (coef == 0 && m.coef == 0) return true;
  for (int i = 0; i < 26; ++i) {
    if (pows[i] != m.pows[i]) return false;
  }
  return true;
}

bool Monomial::operator!=(Monomial m) {
  return !operator==(m);
}

bool Monomial::operator<(Monomial m) {
  for (int i = 0; i < 26; ++i) {
    if (pows[i] != m.pows[i]) return (pows[i] < m.pows[i]);
  }
  return false;
}

bool Monomial::operator<=(Monomial m) {
  for (int i = 0; i < 26; ++i) {
    if (pows[i] != m.pows[i]) return (pows[i] > m.pows[i]);
  }
  return true;
}

bool Monomial::operator>(Monomial m) {
  return (m < *this);
}

bool Monomial::operator>=(Monomial m) {
  return (m <= *this);
}

Monomial Monomial::derivative(ll n, ll p) {
  if (pows[p] < n) return Monomial(0, std::vector<ll>(26, 0));
  Monomial m(*this);
  while (n--) m.coef *= m.pows[p]--;
  return m;
}

Monomial::operator std::string() {
  if (coef == 0) return "0";
  std::string res = std::to_string(coef);
  while (res.back() == '0') res.pop_back();
  if (res.back() == '.') res.pop_back();
  for (int i = 0; i < 26; ++i) {
    if (pows[i] == 0) continue;
    res += 'a' + i;
    if (pows[i] != 1) {
      res += '^';
      res += std::to_string(pows[i]);
    }
  }
  if (coef == 1 && res.size() > 1) res.erase(res.begin());
  else if (coef == -1 && res.size() > 2) res.erase(res.begin() + 1);
  return res;
}

Monomial::operator std::wstring() {
  if (coef == 0) return L"0";
  std::wstring res = std::to_wstring(coef);
  for (int i = 0; i < 26; ++i) {
    if (pows[i] == 0) continue;
    res += 'a' + i;
    res += '^';
    res += std::to_wstring(pows[i]);
  }
  return res;
}

std::stringstream& operator>>(std::stringstream& s, Monomial& m) {
  char c;
  ld z = 0, r = 0, pp = .1;
  std::vector<ll> pows(26, 0);
  int st = 0;
  int p = 0;
  char v = 0;
  bool is_neg = 0;
  while (s >> c) {
    if (st == 0) {
      if (c == '+' || c == '-') {
        st = 1;
        if (c == '-') is_neg = 1;
      } else if (c >= '0' && c <= '9') {
        st = 2;
        z = 10 * z + (c - '0');
      } else if (c == '.') {
        st = 3; 
        pp = .1;
      } else if (c >= 'a' && c <= 'z') {
        z = 1;
        st = 4;
        v = c;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 1) {
      if (c >= '0' && c <= '9') {
        st = 2;
        z = 10 * z + (c - '0');
      } else if (c == '.') {
        st = 3; 
        pp = .1;
      } else if (c >= 'a' && c <= 'z') {
        z = 1;
        st = 4;
        v = c;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 2) {
      if (c >= '0' && c <= '9') {
        st = 2;
        z = 10 * z + (c - '0');
      } else if (c == '.') {
        st = 3; 
        pp = .1;
      } else if (c >= 'a' && c <= 'z') {
        st = 4;
        v = c;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 3) {
      if (c >= '0' && c <= '9') {
        r += pp * (c - '0');
        pp *= .1;
        st = 5;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 4) {
      if (c == '^') {
        p = 0;
        st = 6;
      } else if (c >= 'a' && c <= 'z') {
        ++pows[v - 'a'];
        v = c;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 5) {
      if (c >= '0' && c <= '9') {
        r += pp * (c - '0');
        pp *= .1;
        st = 5;
      } else if (c >= 'a' && c <= 'z') {
        st = 4;
        v = c;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 6) {
      if (c >= '0' && c <= '9') {
        p = p * 10 + (c - '0');
        st = 7;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    } else if (st == 7) {
      if (c >= 'a' && c <= 'z') {
        pows[v - 'a'] += p;
        p = 0;
        v = c;
        st = 4;
      } else if (c >= '0' && c <= '9') {
        p = p * 10 + (c - '0');
        st = 7;
      } else {
        throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
      }
    }
  }
  if (st != 0 && st != 2 && st != 4 && st != 5 && st != 7) throw "WHAT THE FUCK IS THIS PIECE O' SHIT U STUPID FUCKER";
  if (st == 4) {
    ++pows[v - 'a'];
  }
  if (st == 7) pows[v - 'a'] += p;
  m = Monomial((z + r) * (is_neg ? -1 : 1), pows);
  return s;
}

std::ostream& operator<<(std::ostream& s, Monomial m) {
  return s << (std::string)m;
}

std::bitset<26> Monomial::used() {
  std::bitset<26> ans;
  for (int i = 0; i < 26; ++i) ans[i] = (ans[i] || pows[i]);
  return ans;
}
