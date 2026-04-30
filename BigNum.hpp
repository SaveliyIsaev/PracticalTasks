#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <iostream>

using ll = long long;
constexpr ll MOD = __UINT32_MAX__;

class BigNum {
  bool sign;
  std::vector<ll> a;
public:
  BigNum() : a{0} {}
  BigNum(BigNum& x) : sign(x.sign), a(x.a) {}
  BigNum(BigNum&& x) : sign(x.sign), a(std::exchange(x.a, std::vector<ll>())) {}
  BigNum(std::string s, int p = 10) {
   a.push_back(0);
    if (s.empty()) throw "SCHEISSE";
    sign = (s[0] == '-');
    if (s[0] == '-' || s[0] == '+') s.erase(s.begin());
    if (p < 37) {
      for (char &x : s) {
        ll z = 0;
        for (ll i =a.size() - 1; i >= 0; --i) {
          ll f =a[i] * p + z;
          z = f >> 32;
          f &= MOD;
          a[i] = f;
        }
        if (z)a.insert(a.begin(), z);
        z = 0;
        if (x <= '9' && x >= '0') a.back() += x - '0';
        else if (x <= 'Z' && x >= 'A') a.back() += x - 'A' + 10;
        else if (x <= 'z' && x >= 'a') a.back() += x - 'a' + 10;
        else throw "SCHEISSE";
        for (ll i =a.size() - 1; i >= 0; --i) {
          if (z +a[i] > MOD) {
            z = 1;
           a[i] &= MOD;
          } else {
            z = 0;
           a[i] += z;
          }
        }
        if (z)a.insert(a.begin(), z);
      }
      return;
    }
    std::vector<ll> num;
    bool z = 0;
    ll d = 0;
    for (char &c : s) {
      if (c == '[') {
        if (z) throw "SCHEISSE";
        z = 1;
      } else if (c == ']') {
        if (!z) throw "SCHEISSE";
        num.push_back(d);
        d = 0;
        z = 0;
      } else if (c >= '0' && c <= '9') {
        if (!z) throw "SCHEISSE";
        d *= 10;
        d += c - '0';
      } else if (c == ' ') {
        if (z) throw "SCHEISSE";
      } else throw "SCHEISSE";
    }
    for (ll &x : num) {
      ll z = 0;
      for (ll i =a.size() - 1; i >= 0; --i) {
        ll p =a[i] * p + z;
        z = p >> 32;
        p &= MOD;
        a[i] = p;
      }
      if (z) a.insert(a.begin(), z);
      z = 0;
     a.back() += x;
      for (ll i =a.size() - 1; i >= 0; --i) {
        if (z +a[i] > MOD) {
          z = 1;
         a[i] &= MOD;
        } else {
          z = 0;
         a[i] += z;
        }
      }
      if (z)a.insert(a.begin(), z);
    }
  }
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
    while (k.a.size() > 1 && !k.a[0]) k.a.erase(k.a.begin());
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
      ll p = a[i] * y + z;
      z = p >> 32;
      p &= MOD;
      a[i] = p;
    }
    if (z) a.insert(a.begin(), z);
    return *this;
  }
  BigNum operator+=(BigNum& b) {
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
  friend std::istream& operator>>(std::istream& in, BigNum& a) {
    std::string s;
    in >> s;
    a = BigNum(s);
    return in;
  }
  friend BigNum convertfrompto2p32(std::string s, int p = 10) {
    BigNum a;
    // a.a.push_back(0);
    if (s.empty()) throw "SCHEISSE";
    a.sign = (s[0] == '-');
    if (s[0] == '-' || s[0] == '+') s.erase(s.begin());
    if (p < 37) {
      for (char &x : s) {
        ll z = 0;
        for (ll i =a.a.size() - 1; i >= 0; --i) {
          ll f =a.a[i] * p + z;
          z = f >> 32;
          f &= MOD;
          a.a[i] = f;
        }
        if (z) a.a.insert(a.a.begin(), z);
        z = 0;
        if (x <= '9' && x >= '0') a.a.back() += x - '0';
        else if (x <= 'Z' && x >= 'A') a.a.back() += x - 'A' + 10;
        else if (x <= 'z' && x >= 'a') a.a.back() += x - 'a' + 10;
        else throw "SCHEISSE";
        for (ll i = a.a.size() - 1; i >= 0; --i) {
          if (z + a.a[i] > MOD) {
            z = 1;
            a.a[i] &= MOD;
          } else {
            z = 0;
            a.a[i] += z;
          }
        }
        if (z) a.a.insert(a.a.begin(), z);
      }
      return a;
    }
    std::vector<ll> num;
    bool z = 0;
    ll d = 0;
    for (char &c : s) {
      if (c == '[') {
        if (z) throw "SCHEISSE";
        z = 1;
      } else if (c == ']') {
        if (!z) throw "SCHEISSE";
        num.push_back(d);
        d = 0;
        z = 0;
      } else if (c >= '0' && c <= '9') {
        if (!z) throw "SCHEISSE";
        d *= 10;
        d += c - '0';
      } else if (c == ' ') {
        if (z) throw "SCHEISSE";
      } else throw "SCHEISSE";
    }
    for (ll &x : num) {
      ll z = 0;
      for (ll i = a.a.size() - 1; i >= 0; --i) {
        ll f = a.a[i] * p + z;
        z = f >> 32;
        f &= MOD;
        a.a[i] = f;
      }
      if (z) a.a.insert(a.a.begin(), z);
      z = 0;
      a.a.back() += x;
      for (ll i = a.a.size() - 1; i >= 0; --i) {
        if (z + a.a[i] > MOD) {
          z = 1;
          a.a[i] &= MOD;
        } else {
          z = 0;
          a.a[i] += z;
        }
      }
      if (z) a.a.insert(a.a.begin(), z);
    }
    return a;
  }
  friend std::string convertfrom2p32top(BigNum a, int p) {
    if (p < 2) throw "SCHEISSE";
    std::string ans;
    if (a.sign && a.a[0] != 0) ans += '-';
    if (a.a[0] == 0) return ans = "0";
    while (a.a[0]) {
      if (p < 37) {
        int z = a % p;
        if (z < 10) ans.push_back(z + '0');
        else ans += z - 10 + 'A';
      } else ans += " ]" + std::to_string(a % p) + "[";
      a = a / p;
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
  }
  friend std::ostream& operator<<(std::ostream& out, BigNum a) {
    return out << convertfrom2p32top(a, 10);
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
    std::reverse(y.begin(), y.end());
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
      return y * 0;
    }
    if (y.a.empty()) {
      y.a = {0};
      return x * 0;
    }
    if (x.a.size() == 1) return y * x.a[0];
    if (y.a.size() == 1) return x * y.a[0];
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



std::string convert(std::string s, ll p1, ll p2) {
  BigNum b(s, p1);
  return convertfrom2p32top(b, p2);
}
