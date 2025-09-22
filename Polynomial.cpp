#pragma once

#include <sstream>
#include "Polynomial.hpp"
#include "set.cpp"
#include "Monomial.cpp"
#include "List.cpp"
Polynomial::Polynomial() {}
Polynomial::Polynomial(Polynomial& p) : mons(p.mons) {}
Polynomial::Polynomial(Polynomial&& p) {
  mons = std::move(p.mons);
}
Polynomial::Polynomial(set<Monomial>& s) {
  mons = s;
}
Polynomial::Polynomial(set<Monomial>&& s) {
  mons = s;
}
Polynomial::~Polynomial() {}
Polynomial& Polynomial::operator=(Polynomial& m) {
  mons = m.mons;
  return *this;
}
Polynomial& Polynomial::operator=(Polynomial&& m) {
  std::swap(m.mons, mons);
  return *this;
}
Polynomial Polynomial::operator+(Monomial m) {
  Polynomial p = *this;
  p.mons.add(m);
  return p;
}
Polynomial Polynomial::operator-(Monomial m) {
  mons.add(-m);
  return *this;
}

Polynomial Polynomial::operator*(Monomial m) {
  List<Monomial> l = mons;
  set<Monomial> s;
  for (auto x = l.begin; x != nullptr; x = x->next) {
    //std::cout << x->val << ' ' << m << ' '  << x->val * m << std::endl;
    s.add(x->val * m);
  }
  return Polynomial(s);

}
Polynomial Polynomial::operator/(Monomial m) {
  List<Monomial> l = mons;
  set<Monomial> s;
  for (auto x = l.begin; x != nullptr; x = x->next) s.insert(x->val / m);
  std::swap(s, mons);
  return *this;
}
Polynomial& Polynomial::operator+=(Monomial m) {
  mons.add(m);
  return *this;
}
Polynomial& Polynomial::operator-=(Monomial m) {
  mons.add(-m);
  return *this;
}
Polynomial& Polynomial::operator*=(Monomial m) {
  return *this = *this * m;
}
Polynomial& Polynomial::operator/=(Monomial m) {
  return *this = *this / m;
}
Polynomial Polynomial::operator+() {
  return *this;
}
Polynomial Polynomial::operator-() {
  List<Monomial> l = mons;
  set<Monomial> s;
  for (auto x = l.begin; x != nullptr; x = x->next) s.insert(-x->val);
  return Polynomial(s);
}
Polynomial Polynomial::operator+(Polynomial p) {
  List<Monomial> l = p.mons;
  set<Monomial> s = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) s.add(x->val);
  return Polynomial(s);
}
Polynomial Polynomial::operator-(Polynomial p) {
  return *this + -p;
}
Polynomial Polynomial::operator*(Polynomial p) {
  if (size() == 0 || p.size() == 0) return Polynomial();
  List<Monomial> l = p.mons;
  Polynomial s;
  for (auto x = l.begin; x != nullptr; x = x->next) {
    s += *this * x->val;
  } 
  return s;
}
std::pair<Polynomial, Polynomial> Polynomial::operator/(Polynomial p) {
  Polynomial m = *this;
  if (p.size() == 0 || p.size() == 1 && p.mons.find(Monomial(0))) {
    throw "GET THE FUCK OUT OF HERE U SHIT, I WILL NOT DIVIDE THIS BY ZERO, BUT I WILL DIVIDE MY WRATH IF U CONTINUE DOING THIS";
  }
  if (this->size() == 0) {
    return {*this, *this};
  }
  if (used().count() > 1 || p.used().count() > 1) throw "STIRB, DU DUMMES ARSCHLOCH, ICH WERDE KEINE POLYNOME MIT MEHREREN VARIABLEN TEILEN HURE DU BIST DUMM";
  if (used() != p.used() && p.used().count()) throw L"я не буду x на y делить";
  Polynomial ans;
  Monomial x;
  while (!m.mons.empty()) {
    //std::cout << m << std::endl;
    try {
      x = m.mons.mx() / p.mons.mx();
    } catch (const char*) {
      break;
    }
    m -= p * x;
    ans += x;
  }
  return {ans, m};
}
Polynomial& Polynomial::operator+=(Polynomial p) {
  return *this = *this + p;
}
Polynomial& Polynomial::operator-=(Polynomial p) {
  return *this = *this - p;
}
Polynomial& Polynomial::operator*=(Polynomial p) {
  return *this = *this * p;
}
Polynomial& Polynomial::operator/=(Polynomial p) {
  return *this = (*this / p).first;
}
ld Polynomial::operator[](std::vector<ld> v) {
  ld ans = 0;
  List<Monomial> l = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) ans += x->val[v];
  return ans;
}
bool Polynomial::operator==(Polynomial p) {
  List<Monomial> l1 = mons, l2 = p.mons;
  if (l1.begin->val == Monomial(0)) l1.erase(l1.begin);
  if (l2.begin->val == Monomial(0)) l2.erase(l2.begin);
  if (l1.size != l2.size) return false;
  for (auto x = l1.begin, y = l2.begin; x != nullptr; x = x->next, y = y->next) {
    if (x->val != y->val) return false;
  }
  return true;
}
bool Polynomial::operator!=(Polynomial p) {
  return !(*this == p);
} 
Polynomial Polynomial::derivative(ll n, ll p) {
  Polynomial ans;
  List<Monomial> l = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) {
    ans += x->val.derivative(n, p);
  }
  return ans;
}
Polynomial::operator std::string() {
  std::string res;
  List<Monomial> l = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) {
    std::string s = x->val;
    if (s[0] != '-') s = "+ " + s;
    res += " " + s;
  }
  if (!res.empty() && res[0] == ' ') res.erase(res.begin());
  if (!res.empty() && res[0] == '+') res.erase(res.begin(), res.begin() + 2);
  if (res.empty()) return "0";
  return res;
}
Polynomial::operator std::wstring() {
  std::wstring res;
  List<Monomial> l = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) {
    std::wstring s = x->val;
    if (s[0] != '-') s = L"+ ";
    res += L" " + s;
  }
  res.erase(res.begin());
  if (res[0] == L'-') res.erase(res.begin() + 1);
  else res.erase(res.begin(), res.begin() + 2);
  return res;
}

std::bitset<26> Polynomial::used() {
  std::bitset<26> ans;
  List<Monomial> l = mons;
  for (auto x = l.begin; x != nullptr; x = x->next) if (x->val != Monomial(0)) ans |= x->val.used();
  return ans;
}

ll Polynomial::size() {
  return mons.size();
}
std::istream& operator>>(std::istream& s, Polynomial& p) {
  std::string str;
  std::getline(s, str);
  std::stringstream ss;
  Monomial m;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] == ' ') continue;
    if (str[i] == '-' || str[i] == '+') {
      ss >> m;
      p += m;
      ss.clear();
      ss.str("");
      ss.str(ss.str() + str[i]);
    } else ss.str(ss.str() + str[i]);
  }
  ss >> m;
  p += m;
  return s;
}

int main() {
  Polynomial p;
  std::cin >> p;
  std::vector<ld> x(26);
  std::bitset<26> u = p.used();
  for (int i = 0; i < 26; ++i) {
    if (!u[i]) x[i] = 0;
    else {
      std::cout << (char)(i + 'a') << ": ";
      std::cin >> x[i];
    }
  }
  std::cout << p[x];
}
