#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <cassert>
#include <map>
using namespace std;
using ll = long long;
using ull = unsigned long long;
#define all(x) (x).begin(), (x).end()
constexpr ll MOD = ~0u;


class BigNum {
  bool sign = 0;
  vector<ll> a;
public:
  BigNum() : a{0} {}
  BigNum(const BigNum& x) : sign(x.sign), a(x.a) {}
  BigNum(BigNum&& x) : sign(x.sign), a(std::exchange(x.a, vector<ll>())) {}
  BigNum(string s) {
    a.push_back(0);
    if (s.empty()) return;
    sign = (s[0] == '-');
    if (s[0] == '-' || s[0] == '+') s.erase(s.begin());
    for (char &x : s) {
      ll z = 0;
      for (ll i = a.size() - 1; i >= 0; --i) {
        ll p = a[i] * 10ll + z;
        z = p >> 32;
        p &= MOD;
        a[i] = p;
      }
      if (z) a.insert(a.begin(), z);
      z = 0;
      a.back() += x - '0';
      for (ll i = a.size() - 1; i >= 0; --i) {
        if (z + a[i] > MOD) {
          z = 1;
          a[i] &= MOD;
        } else {
          z = 0;
          a[i] += z;
        }
      }
      if (z) a.insert(a.begin(), z);
    }
  }
  BigNum(ll x) {
    if (x < 0) {
      sign = 1;
      x *= -1;
    }
    if (x > MOD) a = {(ll)(x >> 32), (ll)x};
    else a = {x};
  }
  BigNum(bool sign, vector<ll> &x) : sign(sign), a(x) {}
  BigNum(bool sign, vector<ll>&& x) : sign(sign), a(std::exchange(x, vector<ll>())) {}
  BigNum& operator=(BigNum& x) {
    a = x.a;
    sign = x.sign;
    return *this;
  }
  BigNum& operator=(BigNum&& x) {
    sign = x.sign;
    a = std::exchange(x.a, vector<ll>());
    return *this;
  }
  BigNum operator+(BigNum y) {
    if (sign == y.sign) return BigNum(sign, posSum(a, y.a));
    else return *this - y;
  }
  BigNum operator-(BigNum y) {
    if (a.size() < y.a.size() || a.size() == y.a.size() && a < y.a) {
      BigNum z = y - *this;
      return BigNum(!z.sign, std::move(z.a));
    }
    if (sign != y.sign) return BigNum(sign, posSum(a, y.a));
    else return BigNum(sign, posSubs(a, y.a));
  }
  BigNum operator*(BigNum y) {
    BigNum k = Karazuba(*this, y);
    reverse(k.a.begin(), k.a.end());
    while (k.a.size() > 1 && !k.a.back()) k.a.pop_back();
    reverse(k.a.begin(), k.a.end());
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
    vector<ll> res = a;
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
  bool operator!=(BigNum x) {
    return !(*this == x);
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
    if (x == 0) throw "Division by zero";
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
  BigNum& operator/=(BigNum x) {
    return *this = *this / x;
  }
  BigNum& operator-=(BigNum x) {
    return *this = *this - x;
  }
  BigNum operator%(BigNum x) {
    if (x == 0) throw "Division by zero";
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
  BigNum operator%=(BigNum x) {
    return *this = *this % x;
  }
  friend ostream& operator<<(ostream& out, BigNum a) {
    if (a.sign && a.a[0] != 0) out << '-';
    for (auto &x : a.a) cout << x << ' ';
    return out;
    if (a.a[0] == 0) return out << 0;
    string s;
    while (a.a[0] || a.a.size() > 1) {
      s += '0' + a % 10;
      a = a / 10;
    }
    reverse(all(s));
    return out << s;
  }
  friend ll cast_to_ll(const BigNum b) {
    if (b.a.empty()) return 0;
    if (b.a.size() == 1) return b.a[0] * (b.sign ? -1:  1);
    return (b.sign ? -1 : 1) * (b.a[0] + b.a[1] * (MOD + 1));
  }
  private:
  vector<ll> posSum(vector<ll> &x, vector<ll> &y) {
    vector<ll> res;
    reverse(all(x));
    reverse(all(y));
    bool z = 0;
    for (ll i = 0; i < max(x.size(), y.size()); ++i) {
      ll xx = (i < x.size() ? x[i] : 0);
      ll yy = (i < y.size() ? y[i] : 0);
      res.push_back(xx + yy + z & MOD);
      z = xx + yy + z >> 32;
    }
    if (z) res.push_back(1);
    reverse(all(res));
    reverse(all(x));
    reverse(all(y));
    return res;
  }
  vector<ll> posSubs(vector<ll>& x, vector<ll>& y) {
    reverse(all(x));
    reverse(all(y));
    ll z = 0;
    vector<ll> res;
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
    reverse(all(y));
    reverse(all(x));
    while (res.size() > 1 && !res.back()) res.pop_back();
    reverse(all(res));
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
      for (ll x : x.a) if (x != x & MOD) cout << -1 << '\n';
      for (ll z : y.a) {
        b *= (MOD + 1);
        b += x * z;
      }
      return b;
    }
    if (x.a.size() < y.a.size()) {
      vector<ll> z(y.a.size() - x.a.size());
      for (ll x : x.a) z.push_back(x);
      std::swap(x.a, z);
    } else if (x.a.size() > y.a.size()) {
      vector<ll> z(x.a.size() - y.a.size());
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

BigNum gcd(BigNum a, BigNum b) {
  if (a == 0) return b;
  if (b == 0) return a;
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
  Frac(const Frac &f) : n(f.n), m(f.m) {
    BigNum g = gcd(n, m);
    n /= g;
    m /= g;
    if (m < 0) {
      n *= -1;
      m *= -1;
    }
  }
  Frac(Frac &&f) {
    std::swap(n, f.n);
    std::swap(m, f.m);
  }
  Frac(BigNum n, BigNum m = 1) : n(n), m(m) {
    assert(m != 0);
  }
  friend Frac normalize(Frac f) {
    BigNum g = gcd(f.n, f.m);
    f.n /= g;
    f.m /= g;
    if (f.m < 0) {
      f.n *= -1;
      f.m *= -1;
    }
    return f;
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
    BigNum c = lcm(m, b.m);
    BigNum z = n * (c / m) + b.n * (c / b.m);
    Frac zz(z, c);
    return normalize(zz);
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
    return *this = (Frac(n * b.n, m * b.m));
  }
  Frac operator/=(Frac b) {
    assert(b.n != 0);
    return *this *= Frac(b.m, b.n);
  }
};


BigNum convert(std::vector<ll> dig, ll p) {
  if (dig.empty()) return 0;
  if (*std::max_element(dig.begin(), dig.end()) >= p) throw "Wrong digit";
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
  if (!check_factorial(dig)) throw "Wrong digit";
  BigNum b;
  for (ll i = 0; i < dig.size(); ++i) {
    b *= dig.size() - i + 1;
    b += dig[i];
  }
  return b;
}

BigNum convertFromFibonacci(std::vector<ll> dig) {
  if (*std::max_element(dig.begin(), dig.end()) > 1 || *std::min_element(dig.begin(), dig.end()) < 0) throw "Wrong digit";
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

std::vector<ll> convertToBalancedTernary(BigNum b) {
  bool f = (b < 0);
  if (f) b *= -1;
  std::vector<ll> dig = convert(b, 3);
  ll c = 0;
  for (ll i = dig.size() - 1; i >= 0; --i) {
    if (c + dig[i] == 0) continue;
    else if (c + dig[i] == 1) {
      dig[i] = 1;
      c = 0;
    } else if (c + dig[i] == 2) {
      dig[i] = -1;
      c = 1;
    } else {
      dig[i] = 0;
      c = 1;
    }
  }
  if (c == 1) dig.insert(dig.begin(), 1);
  if (f) {
    for (ll& x : dig) x *= -1;
  }
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
      else throw "not a digit";
    } else {
      if (c >= '0' && c <= '9') {
        z *= 10;
        z += c - '0';
        if (z >= p) throw "Wrong digit";
      } else if (c == ']') {
        dig.push_back(z);
        z = 0;
        f = 0;
      } else throw "not a digit";
    }
  }
  return dig;
}

std::vector<ll> extract_balanced_ternary_digits(std::string s) {
  std::vector<ll> dig;
  bool f = 0;
  for (char c : s) {
    if (!f) {
      if (c == '-') f = 1;
      else if (c == '1' || c == '0') dig.push_back(c - '0');
      else throw "Wrong digit";
    } else {
      if (c == '1') {
        dig.push_back(-1);
        f = 0;
      } else throw "Wrong digit";
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
      else throw "not a digit";
    } else {
      if (c >= '0' && c <= '9') {
        z *= 10;
        z += c - '0';
        if (z >= 1e15) throw "Wrong digit";
      } else if (c == ']') {
        dig.push_back(z);
        z = 0;
        f = 0;
      } else throw "not a digit";
    }
  }
  for (ll i = 0; i < dig.size(); ++i) {
    if (dig[i] > dig.size() - i) throw "Wrong digit";
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

void IntegerSolve(std::string a, bool f, ll p1, ll p2) {
  BigNum b;
  std::vector<ll> dig;
  if (p1 < 2 && p1 != -1 && p1 != -2 && p1 != -3 || p2 < 2 && p2 != -1 && p2 != -2 && p2 != -3) throw "Wrong base";
  if (p1 == -3) {
    dig = extract_balanced_ternary_digits(a);
    b = convert(dig, 3);
    cout << b << '\n';
    if (b < 0) {
      f = 1;
      b *= -1;
    }
  } else if (p1 == -2) {
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
  if (p2 == -3) {
    f = 0;
    dig = convertToBalancedTernary(b);
  } else if (p2 == -2) {
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

void print_rational(bool s, BigNum z, Frac f, ll p) {
  if (s) cout << '-';
  cout << construct_number(convert(z, p)) << '.';
  vector<ll> dig;
  map<ll, ll> pos;
  ll per_pos = -1;
  for (ll i = 0;;++i) {
    if (f.n == 0) break;
    f.n *= p;
    BigNum x = f.n / f.m;
    f.n %= f.m;
    if (pos.count(cast_to_ll(f.n))) {
      per_pos= pos[cast_to_ll(f.n)];
      break;
    }
    dig.push_back(cast_to_ll(x));
    pos[cast_to_ll(f.n)] = i;
  }
  if (per_pos == -1) {
    for (ll x : dig) {
      if (x < 10) cout << (char)('0' + x);
      else if (x < 36) cout << (char)(x - 10 + 'A');
      else cout << '[' << x << ']';
    }
    std::cout << std::endl;
  } else {
    for (ll i = 0; i < per_pos; ++i) {
      if (dig[i] < 10) cout << (char)('0' + dig[i]);
      else if (dig[i] < 36) cout << (char)(dig[i] - 10 + 'A');
      else cout << '[' << dig[i] << ']';
    }
    cout << '(';
    for (ll i = per_pos; i < dig.size(); ++i) {
      if (dig[i] < 10) cout << (char)('0' + dig[i]);
      else if (dig[i] < 36) cout << (char)(dig[i] - 10 + 'A');
      else cout << '[' << dig[i] << ']';
    }
    cout << ')' << std::endl;
  }
}

void Solve() {
  std::string a; 
  bool f = 0;
  ll p1, p2;
  std::cin >> a >> p1 >> p2;
  if (a[0] == '-' && p1 != -3) {
    f = 1;
    a.erase(a.begin());
  } else if (a[0] == '+') a.erase(a.begin());
  if (is_int(a)) {
    IntegerSolve(a, f, p1, p2);
    return;
  }
  if (p1 < 2 || p2 < 2) throw "Wrong base";
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
  if (i != a.size() - 1 && std::find(a.begin(), a.end(), '(') != a.end() || std::find(a.begin(), a.end(), '(') == a.end() && i < a.size()) throw "idk how to describe it? but ur wrong";
  BigNum zn, sbp, pn;
  if (p.empty()) {
    zn = convert(extract_digits(z, p1), p1);
    sbp = convert(extract_digits(subp, p1), p1);
    Frac sbf(sbp, fastpow(BigNum(p1), subp.size())); 
    print_rational(f, zn, sbf, p2);
    return;
  }
  zn = convert(extract_digits(z, p1), p1);
  sbp = convert(extract_digits(subp, p1), p1);
  Frac sbf(sbp, fastpow(BigNum(p1), subp.size()));
  pn = convert(extract_digits(p, p1), p1);
  auto w = (fastpow(BigNum(p1), p.size()) - 1) * fastpow(BigNum(p1), subp.size());
  Frac per(pn,w );
  Frac d = sbf + per;
  print_rational(f, zn, d, p2);
}

int main() {
  for (;;) {
    try {
      Solve();
    } catch(char const* s) {
      std::cout << s << std::endl;
    }
  }
}
