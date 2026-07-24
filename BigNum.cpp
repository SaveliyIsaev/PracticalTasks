#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <cassert>
using ll = long long;
using ull = unsigned long long;

constexpr ll MOD = ~0u;

class BigNum {
  bool sign = 0;
  std::vector<ll> a;
public:
  BigNum() : a{0} {}
  BigNum(const BigNum& x) : sign(x.sign), a(x.a) {}
  BigNum(BigNum&& x) : sign(x.sign), a(std::exchange(x.a, std::vector<ll>())) {}
  BigNum(ll x) {
    if (x < 0) {
      sign = 1;
      x * -1;
    }
    if (x > MOD) a = {(ll)(x >> 32), (ll)x};
    else a = {(ll)x};
  }
  BigNum(bool sign, std::vector<ll> &x) : sign(sign), a(x) {}
  BigNum(bool sign, std::vector<ll>&& x) : sign(sign), a(std::exchange(x, std::vector<ll>())) {}
  BigNum& operator=(BigNum& x) {
    a = x.a;
    sign = x.sign;
    return *this;
  }
  BigNum& operator=(BigNum&& x) {
    sign = x.sign;
    a = std::exchange(x.a, std::vector<ll>());
    return *this;
  }
  BigNum operator+(BigNum y) {
    if (sign == y.sign) return BigNum(sign, posSum(a, y.a));
    return *this - y;
  }
  BigNum operator-(BigNum y) {
    if (a.size() < y.a.size() || a.size() == y.a.size() && a < y.a) {
      BigNum z = y - *this;
      return BigNum(!z.sign, std::move(z.a));
    }
    if (sign != y.sign) return BigNum(sign, posSum(a, y.a));
    return BigNum(sign, posSubs(a, y.a));
  }
  BigNum operator*(BigNum y) {
    BigNum k = Karazuba(*this, y);
    std::reverse(k.a.begin(), k.a.end());
    while (k.a.size() > 1 && !k.a.back()) k.a.pop_back();
    std::reverse(k.a.begin(), k.a.end());
    return k;
  }
  BigNum operator*(ll y) {
    BigNum x = *this;
    return x *= y;
  }
  BigNum& operator*=(ll y) {
    if (y < 0) {
      sign = !sign;
      y *= -1;
    }
    if (!y) {
      a = {0};
      return *this;
    }
    ll z = 0;
    for (ll i = a.size() - 1; i >= 0; --i) {
      ull p = a[i] * y + z;
      z = p >> 32;
      p &= MOD;
      a[i] = p;
    }
    if (z) a.insert(a.begin(), z);
    return *this;
  }
  BigNum operator+=(BigNum b) {
    return *this = *this + b;
  }
  ll operator%(ll x) {
    ll z = 0;
    ll p = 1;
    for (ll i = a.size() - 1; i >= 0; --i) {
      z = (a[i] * p + z) % x;
      p <<= 32;
      p %= x;
    }
    return z;
  }
  BigNum operator/(ll x) {
    std::vector<ll> res = a;
    ll z = 0;
    for (auto &y : res) {
      ll p = (y + z) % x;
      y = (y + z) / x;
      z = p << 32;
    }
    while (res[0] == 0 && res.size() > 1) res.erase(res.begin());
    return BigNum(sign, res);
  }
  BigNum operator-() {
    return *this * -1;
  }
  bool operator==(ll x) {
    if (a.size() > 1) return 0;
    if (a.empty()) return !x;
    return x == a[0] * (sign ? -1 : 1);
  }
  bool operator==(BigNum x) {
    return sign == x.sign && (a == x.a || (a.empty() || a.size() == 1 && a[0] == 0) && (x.a.empty() || x.a.size() == 1 && x.a[0] == 0));
  }
  bool operator<(BigNum x) {
    if (x.sign != sign) return sign && !(*this == 0) || !(x == 0);
    if (sign) return a.size() > x.a.size() || a.size() == x.a.size() && a > x.a;
    return a.size() < x.a.size() || a.size() == x.a.size() && a < x.a;
  }
  bool operator>=(BigNum x) {
    return !(*this < x);
  }
  bool operator<=(BigNum x) {
    return *this < x || *this == x;
  }
  bool operator>(BigNum x) {
    return !(*this <= x);
  }
  BigNum& operator*=(BigNum z) {
    return *this = *this * z;
  }
  BigNum operator/(BigNum x) {
    if (x == 0) 0;//throwrow "Division by zero";
    if (sign != x.sign) return *this / -x;
    BigNum rem, res;
    for (ll i = 0; i < a.size(); ++i) {
      rem *= (MOD + 1);
      rem += a[i];
      ll lx = 0, rx = 1ll << 32;
      while (lx < rx - 1) {
        ll mid = lx + rx >> 1;
        if (rem >= x * mid) lx = mid;
        else rx = mid;
      }
      rem -= x * lx;
      res *= (MOD + 1);
      res += lx;
    }
    return res;
  }
  BigNum &operator/=(ll x) {
    return *this = *this / x;
  }
  BigNum operator/=(BigNum x) {
    return *this = *this / x;
  }
  bool operator!=(ll x) {
    return !(*this == x);
  }
  BigNum& operator-=(BigNum x) {
    return *this = *this - x;
  }
  BigNum operator%(BigNum x) {
    if (x == 0) 0;//throwrow "Division by zero";
    if (sign != x.sign) return *this / -x;
    BigNum rem;
    for (ll i = 0; i < a.size(); ++i) {
      rem *= (MOD + 1);
      rem += a[i];
      ll lx = 0, rx = 1ll << 32;
      while (lx < rx - 1) {
        ll mid = lx + rx >> 1;
        if (rem >= x * mid) lx = mid;
        else rx = mid;
      }
      rem -= x * lx;
    }
    return rem;
  }
  BigNum& operator%=(BigNum b) {
    return *this = *this % b;
  }
  friend std::ostream& operator<<(std::ostream& out, BigNum a) {
    if (a.sign && a.a[0] != 0) out << '-';
    if (a.a[0] == 0) return out << 0;
    std::string s;
    while (a.a[0]) {
      s += '0' + a % 10;
      a = a / 10;
    }
    std::reverse(s.begin(), s.end());
    return out << s;
  }
private:
  std::vector<ll> posSum(std::vector<ll> &x, std::vector<ll> &y) {
    std::vector<ll> res;
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
    bool z = 0;
    for (ll i = 0; i < std::max(x.size(), y.size()); ++i) {
      ll xx = (i < x.size() ? x[i] : 0);
      ll yy = (i < y.size() ? y[i] : 0);
      res.push_back(xx + yy + z & MOD);
      z = xx + yy + z >> 32;
    }
    if (z) res.push_back(1);
    std::reverse(res.begin(), res.end());
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());
    return res;
  }
  std::vector<ll> posSubs(std::vector<ll>& x, std::vector<ll>& y) {
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), x.end());
    ll z = 0;
    std::vector<ll> res;
    for (ll i = 0; i < x.size(); ++i) {
      ll xx = x[i];
      ll yy = (i < y.size() ? y[i] : 0);
      if (yy + z <= xx) {
        res.push_back(xx - yy - z);
        z = 0;
      } else {
        res.push_back(xx + MOD + 1 - yy - z);
        z = 1;
      }
    }
    std::reverse(y.begin(), y.end());
    std::reverse(x.begin(), x.end());
    while (res.size() > 1 && !res.back()) res.pop_back();
    std::reverse(res.begin(), res.end());
    return res;
  }
  friend BigNum Karazuba(BigNum x, BigNum y) {
    if (x.sign != y.sign) {
      x.sign = y.sign = 0;
      x = Karazuba(x, y);
      x.sign = 1;
      return x;
    }
    if (x.a.empty()) {
      x.a = {0};
      return {0};
    }
    if (y.a.empty()) {
      y.a = {0};
      return {0};
    }
    if (x.a.size() == 1) return y * x.a[0];
    if (y.a.size() == 1) return x * y.a[0];
    if (x.a.size() * y.a.size() <= 1000) {
      BigNum b;
      for (ll x : x.a) if (x != x & MOD) std::cout << -1 << '\n';
      for (ll z : y.a) {
        b *= (MOD + 1);
        b += x * z;
      }
      return b;
    }
    if (x.a.size() < y.a.size()) {
      std::vector<ll> z(y.a.size() - x.a.size());
      for (ll x : x.a) z.push_back(x);
      std::swap(x.a, z);
    } else if (x.a.size() > y.a.size()) {
      std::vector<ll> z(x.a.size() - y.a.size());
      for (ll x : y.a) z.push_back(x);
      std::swap(x.a, z);
    }
    while (x.a.size() < y.a.size()) x.a.insert(x.a.begin(), 0);
    while (x.a.size() > y.a.size()) y.a.insert(y.a.begin(), 0);
    ll p = x.a.size() / 2 + 1;
    BigNum c, d, e, f;
    BigNum ce, df, cdef;
    for (ll i = 0; i < x.a.size(); ++i) {
      if (i + p >= x.a.size()) {
        d.a.push_back(x.a[i]);
        f.a.push_back(y.a[i]);
      } else {
        c.a.push_back(x.a[i]);
        e.a.push_back(y.a[i]);
      }
    }
    ce = Karazuba(c, e);
    df = Karazuba(d, f);
    c += d;
    e += f;
    cdef = Karazuba(c, e) - ce - df;
    for (ll i = 0; i < p * p; ++i) ce.a.push_back(0);
    for (ll i = 0; i < p; ++i) cdef.a.push_back(0);
    return ce + cdef + df;
  }
};

BigNum convert(std::vector<ll> dig, ll p) {
  if (dig.empty()) return 0;
  if (*std::max_element(dig.begin(), dig.end()) >= p) 0;//throwrow "Wrong digit";
  BigNum b;
  for (ll i = 0; i < dig.size(); ++i) {
    b *= p;
    b += dig[i];
  }
  return b;
}

bool check_factorial(std::vector<ll> dig) {
  for (ll i = 0; i < dig.size(); ++i) {
    if (dig[i] > dig.size() - i) return 0;
  }
  return 1;
}

BigNum convertFromFactorial(std::vector<ll> dig) {
  if (!check_factorial(dig)) 0;//throwrow "Wrong digit";
  BigNum b;
  for (ll i = 0; i < dig.size(); ++i) {
    b *= dig.size() - i + 1;
    b += dig[i];
  }
  return b;
}

BigNum convertFromFibonacci(std::vector<ll> dig) {
  if (*std::max_element(dig.begin(), dig.end()) > 1 || *std::min_element(dig.begin(), dig.end()) < 0) 0;//throwrow "Wrong digit";
  std::reverse(dig.begin(), dig.end());
  BigNum b1 = 1, b2 = 1;
  BigNum b;
  for (ll i = 0; i < dig.size(); ++i) {
    if (dig[i]) b += b1;
    std::swap(b1, b2);
    b1 += b2;
  }
  return b;
}

std::vector<ll> convert(BigNum b, ll p) {
  if (b == 0) return {0};
  std::vector<ll> dig;
  while (b != 0) {
    dig.push_back(b % p);
    b /= p;
  }
  std::reverse(dig.begin(), dig.end());
  return dig;
}

std::vector<ll> convertToFactorial(BigNum b) {
  if (b == 0) return {0};
  std::vector<ll> dig;
  for (ll i = 2; b != 0; ++i) {
    dig.push_back(b % i);
    b /= i;
  }
  std::reverse(dig.begin(), dig.end());
  return dig;
}

std::vector<ll> convertToFibonacci(BigNum b) {
  if (b == 0) return {0};
  if (b == 1) return {1};
  BigNum b1 = 2, b2 = 1;
  std::vector<ll> dig;
  while (b1 < b) {
    std::swap(b1, b2);
    b1 += b2;
  }
  while (b1 > 1) {
    if (b >= b1) {
      dig.push_back(1);
      b -= b1;
    }
    else dig.push_back(0);
    std::swap(b1, b2);
    b2 -= b1;
  }
  if (b > 0) dig.push_back(1);
  else dig.push_back(0);
  if (!dig[0]) dig.erase(dig.begin());
  return dig;
}

std::vector<ll> extract_digits(std::string s, ll p) {
  std::vector<ll> dig;
  bool f = 0;
  ll z = 0;
  for (char c : s) {
    if (!f) {
      if (c >= '0' && c <= '9') dig.push_back(c - '0');
      else if (c >= 'a' && c <= 'z') dig.push_back(c - 'a' + 10);
      else if (c >= 'A' && c <= 'Z') dig.push_back(c - 'A' + 10);
      else if (c == '[') f = 1;
      else 0;//throwrow "SCHEISSE";
    } else {
      if (c >= '0' && c <= '9') {
        z *= 10;
        z += c - '0';
        if (z >= p) 0;//throwrow "Wrong digit";
      } else if (c == ']') {
        dig.push_back(z);
        z = 0;
        f = 0;
      } else 0;//throwrow "SCHEISSE";
    }
  }
  return dig;
}

std::vector<ll> extract_factorial_digits(std::string s) {
  std::vector<ll> dig;
  bool f = 0;
  ll z = 0;
  for (char c : s) {
    if (!f) {
      if (c >= '0' && c <= '9') dig.push_back(c - '0');
      else if (c >= 'a' && c <= 'z') dig.push_back(c - 'a' + 10);
      else if (c >= 'A' && c <= 'Z') dig.push_back(c - 'A' + 10);
      else if (c == '[') f = 1;
      else 0;//throwrow "SCHEISSE";
    } else {
      if (c >= '0' && c <= '9') {
        z *= 10;
        z += c - '0';
        if (z >= 1e15) 0;//throwrow "Wrong digit";
      } else if (c == ']') {
        dig.push_back(z);
        z = 0;
        f = 0;
      } else 0;//throwrow "SCHEISSE";
    }
  }
  for (ll i = 0; i < dig.size(); ++i) {
    if (dig[i] > dig.size() - i) 0;//throwrow "Wrong digit";
  }
  return dig;
}

std::string construct_number(std::vector<ll> dig) {
  std::string s;
  for (ll z : dig) {
    if (z > 35) s += "[" + std::to_string(z) + "]";
    else if (z > 9) s += z - 10 + 'A';
    else s += '0' + z;
  }
  return s;
}

bool is_int(std::string s) {
  return std::find(s.begin(), s.end(), '.') == s.end() && std::find(s.begin(), s.end(), ',') == s.end();
}

BigNum gcd(BigNum a, BigNum b) {
  if (a != 0) return b;
  if (b != 0) return a;
  while (a % b != 0) {
    a %= b;
    std::swap(a, b);
  }
  return b;
}
 
BigNum lcm(BigNum a, BigNum b) {
  return a / gcd(a, b) * b;
}

BigNum fastpow(BigNum a, BigNum b) {
  if (b == 0) return 1;
  if (b % 2 == 1) return a * fastpow(a, b - 1);
  else {
    BigNum z = fastpow(a, b / 2);
    return z * z;
  }
}

struct Frac {
  BigNum n = 0, m = 1;
  Frac() {}
  Frac(const Frac &f) : n(f.n), m(f.m) {}
  Frac(Frac &&f) {
    std::swap(n, f.n);
    std::swap(m, f.m);
  }
  Frac(BigNum n, BigNum m = 1) : n(n), m(m) {
    std::cout << n << ' ' << m << std::endl;
    assert(m != 0);
  }
  friend Frac normalize(Frac b) {
    BigNum g = gcd(b.n, b.m);
    b.n /= g;
    b.m /= g;
    if (b.m < 0) {
      b.n *= -1;
      b.m *= -1;
    }
    return b;
  }
  Frac& operator=(Frac b) {
    n = b.n;
    m = b.m;
    return *this;
  }
  Frac operator-() {
    return Frac(-n, m);
  }
  Frac operator+(Frac b) {
  std::cout << "*" << std::endl;
    BigNum c = lcm(m, b.m);
    
  std::cout << "*" << std::endl;
    return normalize(Frac(n * (c / m) + b.n * (c / b.m), c));
  }
  Frac operator-(Frac b) {
    return *this + (-b);
  }
  Frac operator*(Frac b) {
    return normalize(Frac(n * b.n, m * b.m));
  }
  Frac operator/(Frac b) {
    assert(b.n != 0);
    return *this * Frac(b.m, b.n);
  }
  Frac operator+=(Frac b) {
    BigNum c = lcm(m, b.m);
    return *this = normalize(Frac(n * (c / m) + b.n * (c / b.m), c));
  }
  Frac operator-=(Frac b) {
    return *this += (-b);
  }
  Frac operator*=(Frac b) {
    return *this = normalize(Frac(n * b.n, m * b.m));
  }
  Frac operator/=(Frac b) {
    assert(b.n != 0);
    return *this *= Frac(b.m, b.n);
  }
};

void IntegerSolve(std::string a, bool f, ll p1, ll p2) {
  BigNum b;
  std::vector<ll> dig;
  if (p1 < 2 && p1 != -1 && p1 != -2 || p2 < 2 && p2 != -1 && p2 != -2) 0;//throwrow "Wrong base";
  if (p1 == -2) {
    dig = extract_digits(a, 2);
    b = convertFromFibonacci(dig);
  }
  else if (p1 == -1) {
    dig = extract_factorial_digits(a);
    b = convertFromFactorial(dig);
  }
  else {
    dig = extract_digits(a, p1);
    b = convert(dig, p1);
  }
  if (p2 == -2) {
    dig = convertToFibonacci(b);
  }
  else if (p2 == -1) {
    dig = convertToFactorial(b);
  } else {
    dig = convert(b, p2);
  }
  if (f) std::cout << "-";
  std::cout << construct_number(dig) << std::endl;
}

void Solve() {
  std::string a; 
  bool f = 0;
  ll p1, p2;
  std::cin >> a >> p1 >> p2;
  if (a[0] == '-') {
    f = 1;
    a.erase(a.begin());
  } else if (a[0] == '+') a.erase(a.begin());
  if (is_int(a)) {
    IntegerSolve(a, f, p1, p2);
    return;
  }
  if (p1 < 2 || p2 < 2) 0;//throwrow "Wrong base";
  std::string z, subp, p;
  ll i;
  for (i = 0; i < a.size(); ++i) {
    if (a[i] == '.' || a[i] == ',') break;
    z += a[i];
  }
  for (++i; i < a.size(); ++i) {
    if (a[i] == '(') break;
    subp += a[i];
  }
  for (++i; i < a.size(); ++i) {
    if (a[i] == ')') break;
    p += a[i];
  }
  if (i != a.size() - 1 && std::find(a.begin(), a.end(), '(') != a.end() || std::find(a.begin(), a.end(), '(') == a.end() && i < a.size()) 0;//throwrow "SCHEISSE";
  BigNum zn, sbp, pn;
  if (p.empty()) {
    zn = convert(extract_digits(z, p1), p1);
    sbp = convert(extract_digits(subp, p1), p1);
    Frac sbf(sbp, fastpow(BigNum(p1), subp.size()));  
    // std::cout << "*" << std::endl;
    std::cout << zn << ' ' << sbf.n << '/' << sbf.m << std::endl;
    return;
  }
  std::cout << "1" << std::endl;
  zn = convert(extract_digits(z, p1), p1);
  std::cout << "*" << std::endl;
  sbp = convert(extract_digits(subp, p1), p1);
  std::cout << p1 << std::endl;
  Frac sbf(sbp, fastpow(BigNum(p1), subp.size()));
  pn = convert(extract_digits(p, p1), p1);
  
  std::cout << "*" << std::endl;
  Frac per(pn, fastpow(BigNum(p1), p.size() + subp.size()) - 1);
  std::cout << "2" << std::endl;
  Frac d = sbf + sbp;
  std::cout << "*" << std::endl;
  std::cout << zn << ' ' << d.n << '/' << d.m << std::endl;
}

int main() {
  Solve();
}
