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
        throw "СЪЕБИ НАХУЙ ОТСЮДА, ЕДИСТВЕННОЕ, ЧТО Я БУДУ ДЕЛАТЬ С НУЛЕМ - ЭТО ДОМНОЖАТЬ БОШКУ ТВОЮ НА НЕГО";
    }
    if (this->size() == 0) {
        return {*this, *this};
    }
    if (used().count() > 1 || p.used().count() > 1) throw "STIRB, DU DUMMES ARSCHLOCH, ICH WERDE KEINE POLYNOME MIT MEHREREN VARIABLEN TEILEN HURE DU BIST DUMM";
    if (used() != p.used() && p.used().count() && used().count()) throw L"ИДИ НАХУЙ, Я НЕ БУДУ Х НА У ДЕЛИТЬ";
    Polynomial ans;
    Monomial x;
    while (!m.mons.empty()) {
        //std::cout << m << std::endl;
        try {
            x = m.mons.mx() / p.mons.mx();
        } catch (const char*) {
            break;
        }
        m -= (p * x);
        ans += x;
    }
    return {ans, m};
}
Polynomial& Polynomial::operator+=(Polynomial p) {
    return *this = *this + p;
}
Polynomial& Polynomial::operator-=(Polynomial p) {
    *this += -p;
    return *this;
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
        else s.insert(s.begin() + 1, ' ');
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
    p = Polynomial();
    std::string str;
    std::getline(s, str);
    std::stringstream ss;
    Monomial m;
    char lst = 0;
    bool sp = 1;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            sp = 1;
            continue;
        } else {
            if (lst >= '0' && lst <= '9' && sp && str[i] >= '0' && str[i] <= '9') {
                throw "ЧЕ ЗА ХУЙНЮ ТЫ ВВЕЛ ЕПТА";
            }
            lst = str[i];
            sp = 0;
        }
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

std::vector<ll> Polynomial::roots() {
    if (used().count() > 1) {
        throw "ДА БЛЯ, Я НЕ БУДУ КОРНИ ИЗ ТЫСЯЧИ ПЕРЕМЕННЫХ ХУЯРИТЬ";
    }
    if (mons.size() == 0) {
        return {INT64_MAX};
    }
    Polynomial pl = *this;
    std::vector<ll> ans;
    Monomial m;
    if ((m = pl.mons.mn()).used() != 0) {
        ans.push_back(0);
        pl /= Monomial(1, m.getPows());
    }
    while (m.getCoef() != (ll)m.getCoef()) {
        pl *= 10;
        m *= 10;
    }
    ll x = m.getCoef();
    std::vector<ld> pws(26);
    std::vector<ll> p = mons.mx().getPows();
    ll z;
    for (z  = 0; z < 26; ++z) {
        if (p[z]) break;
    }
    for (int i = 1; i * i <= abs(x); ++i) {
        if (x % i) continue;
        pws[z] = i;
        if (pl[pws] == 0) ans.push_back(i);
        pws[z] = -i;
        if (pl[pws] == 0) ans.push_back(-i);
        pws[z] = x / i;
        if (pl[pws] == 0) ans.push_back(x / i);
        pws[z] = -x / i;
        if (pl[pws] == 0) ans.push_back(-x / i);
    }
    return ans;
}
